#include "GameController.h"
#include "Resources.h"
#include "Level.h"
#include "Player.h"
#include "Goomba.h"
#include "Coin.h"

#include "PhysicsEngine.h"
#include "Timer.h"

#include <filesystem>

#include <SFML/Audio.hpp>

Level m_level(1.0f);
Camera g_Camera(30.0f);
Player m_player{};
GameState gameState;
Timer m_timer;
Timer* g_GlobalTimer = nullptr;

bool g_IsGamePaused;

std::vector<GameObject*> gameObjects;

sf::Music gameMusic{};
sf::Sound gameOverSoundEffect{};
sf::Sound pauseSoundEffect{};

sf::Image levelImage;
sf::Font gameFont{};

sf::Text coinsText("Coins: ", gameFont);
sf::Text timerText("Time: ", gameFont);
sf::Text jumpFuelText("JumpFuel: ", gameFont);
sf::Text gameOverText("Game Over", gameFont);
sf::Text restartText("Press Enter to Restart", gameFont);
sf::Text pauseText("Game Paused", gameFont);
sf::Text titleText("MARIOS BROS DOWNWELL", gameFont);
sf::Text pressPlayText("Press X to Start", gameFont);

sf::RectangleShape backgroundShape(sf::Vector2f(1.0f, 1.0f));

void RestartGame()
{	
	gameMusic.stop();

	PhysicsEngine::Cleanup();
	PhysicsEngine::Initialize();

	m_player = Player();
	m_player.playerPosition = m_level.CreateLevelByImage(levelImage, gameObjects);
	m_player.isDead = false;

	g_IsGamePaused = false;
	
	m_player.InitializePlayer();

	for (auto& object : gameObjects)
	{
		object->Initialize();
	}

	gameMusic.play();

	gameState = PLAYING;

	m_timer.SetTimeLimit(60.0f);

}

void InitializeGame()
{	
	gameState = MAIN_MENU;

	LoadResources();

	gameMusic.openFromFile("./resources/audio/music/music.ogg");
	gameMusic.setLoop(true);
	gameMusic.setVolume(50.0f);

	g_GlobalTimer = &m_timer;
	
	gameFont.loadFromFile("./resources/fonts/super-mario-bros-nes.ttf");
	coinsText.setFillColor(sf::Color::White);
	coinsText.setOutlineColor(sf::Color::Black);
	coinsText.setOutlineThickness(2.4f);
	coinsText.setScale(0.1f, 0.1f); 

	timerText.setFillColor(sf::Color::White);
	timerText.setOutlineColor(sf::Color::Black);
	timerText.setOutlineThickness(2.4f);
	timerText.setScale(0.1f, 0.1f);

	jumpFuelText.setFillColor(sf::Color::White);
	jumpFuelText.setOutlineColor(sf::Color::Black);
	jumpFuelText.setOutlineThickness(2.4f);
	jumpFuelText.setScale(0.1f, 0.1f);

	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setOutlineColor(sf::Color::Black);
	gameOverText.setOutlineThickness(1.0f);
	gameOverText.setScale(0.1f, 0.1f);

	restartText.setFillColor(sf::Color::White);
	restartText.setOutlineColor(sf::Color::Black);
	restartText.setOutlineThickness(1.0f);
	restartText.setScale(0.1f, 0.1f);

	pauseText.setFillColor(sf::Color::White);
	pauseText.setOutlineColor(sf::Color::Black);
	pauseText.setOutlineThickness(1.0f);
	pauseText.setScale(0.1f, 0.1f);

	titleText.setFillColor(sf::Color::White);
	titleText.setCharacterSize(40);
	titleText.setOutlineColor(sf::Color::Black);
	titleText.setOutlineThickness(1.0f);
	titleText.setScale(0.1f, 0.1f);

	pressPlayText.setFillColor(sf::Color::White);
	pressPlayText.setOutlineColor(sf::Color::Black);
	pressPlayText.setOutlineThickness(1.0f);
	pressPlayText.setScale(0.1f, 0.1f);

	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setOrigin(0.5f, 0.5f);

	levelImage.loadFromFile("levelOne.png");

}

void Update(float deltaTime)
{

	if (gameState == MAIN_MENU)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			RestartGame();
		}
	}
	else if (gameState == GAME_OVER)
	{
		gameMusic.stop();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			RestartGame();
		}
	}
	else if (gameState == PLAYING)
	{
		if (m_player.isDead)
		{
			gameState = GAME_OVER;
		}
		else if (m_timer.IsTimeUp())
		{
			gameState = GAME_OVER;
		}

		if (!m_player.isDead && !g_IsGamePaused)
		{
			PhysicsEngine::Update(deltaTime);
			m_player.Update(deltaTime);
			m_timer.Update();

			g_Camera.cameraPosition = sf::Vector2f(25, m_player.playerPosition.y - 3.0f);

			for (auto& object : gameObjects)
			{
				object->UpdateGameObject(deltaTime);
			}

			//m_timer.DisplayTime();
		}
	}

}

void RenderGameWindow(TextureController& textureController)
{
	m_level.Render(textureController);
	m_player.RenderPlayer(textureController);

	for (auto& object : gameObjects)
	{
		object->RenderGameObject(textureController);
	}
}

void RenderGameUI(TextureController& textureController)
{
	coinsText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(2.0f, 1.0f));
	coinsText.setString("Coins:" + std::to_string(m_player.GetCoins()));
	textureController.renderTarget.draw(coinsText);
	
	timerText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(77.0f, 1.0f));
	int currentTimeInt = static_cast<int>(m_timer.GetCurrentTime());
	timerText.setString("Time:" + std::to_string(currentTimeInt));
	textureController.renderTarget.draw(timerText);

	jumpFuelText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(32.0f, 1.0f));
	int currentJumpFuelInt = static_cast<int>(m_player.GetJumpFuel());
	jumpFuelText.setString("JumpFuel:" + std::to_string(currentJumpFuelInt));
	textureController.renderTarget.draw(jumpFuelText);

	if (gameState == MAIN_MENU)
	{
		backgroundShape.setScale(g_Camera.GetViewSize());
		textureController.renderTarget.draw(backgroundShape);
		titleText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(10.0f, 25.0f));
		pressPlayText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(25.0f, 35.0f));
		textureController.renderTarget.draw(titleText);
		textureController.renderTarget.draw(pressPlayText);
	}

	else if (g_IsGamePaused && !m_player.isDead)
	{
		backgroundShape.setScale(g_Camera.GetViewSize());
		textureController.renderTarget.draw(backgroundShape);
		pauseText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(32.0f, 25.0f));
		textureController.renderTarget.draw(pauseText);
	}

	else if (m_player.isDead  && !m_player.hasWon || m_timer.IsTimeUp() && !m_player.hasWon)
	{
		backgroundShape.setScale(g_Camera.GetViewSize());
		textureController.renderTarget.draw(backgroundShape);

		gameOverText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(35.0f, 25.0f));
		textureController.renderTarget.draw(gameOverText);

		restartText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(18.0f, 35.0f));
		textureController.renderTarget.draw(restartText);
	}

	else if (m_player.isDead && m_player.hasWon)
	{
		backgroundShape.setScale(g_Camera.GetViewSize());
		textureController.renderTarget.draw(backgroundShape);

		gameOverText.setString("Congratulations! You Won!");
		gameOverText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(13.0f, 25.0f));
		textureController.renderTarget.draw(gameOverText);

		restartText.setPosition(-g_Camera.GetViewSize() / 2.0f + sf::Vector2f(18.0f, 35.0f));
		textureController.renderTarget.draw(restartText);
	}

}

void DeleteGameObject(GameObject* object)
{
	const auto& objectToDelete = std::find(gameObjects.begin(), gameObjects.end(), object);
	if (objectToDelete != gameObjects.end())
	{
		delete* objectToDelete;
		gameObjects.erase(objectToDelete);
	}
}

void LoadResources()
{
	for (auto& file : std::filesystem::directory_iterator("./resources/textures/"))
	{
		if (file.is_regular_file() && file.path().extension() == ".png")
		{
			Resources::textures[file.path().filename().string()].
				loadFromFile(file.path().string());
		}
	}

	for (auto& file : std::filesystem::directory_iterator("./resources/audio/music/"))
	{
		if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
		{
			Resources::audio[file.path().filename().string()].loadFromFile(
				file.path().string());
		}
	}

	for (auto& file : std::filesystem::directory_iterator("./resources/audio/soundEffects/"))
	{
		if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
		{
			Resources::audio[file.path().filename().string()].loadFromFile(
				file.path().string());
		}
	}
}
 