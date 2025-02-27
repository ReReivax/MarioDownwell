#include <SFML/Graphics.hpp>
#include "GameController.h"
#include "TextureController.h"
#include "Camera.h"


int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 900), "Mario Bros Downwell");
	sf::Clock deltaClock;
	TextureController textureController(window);
	window.setFramerateLimit(60);
	InitializeGame();

	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				g_IsGamePaused = !g_IsGamePaused;
		}

		Update(deltaTime);
		window.clear(sf::Color(4, 156, 216));

		window.setView(g_Camera.GetCameraView(window.getSize()));
		RenderGameWindow(textureController);

		window.setView(g_Camera.GetUIView());
		RenderGameUI(textureController);

		window.display();
	}
}