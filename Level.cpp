#include "Level.h"
#include "Resources.h"
#include "PhysicsEngine.h"
#include "Coin.h"
#include "Goomba.h"
#include "BulletBill.h"
#include "PiranhaPlant.h"
#include "Star.h"
#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>

Level::Level(float cellSize)
	: cellSize(cellSize), grid()
{}

sf::Vector2f Level::CreateLevelByImage(const sf::Image& image, std::vector<GameObject*>& objects)
{
	objects.clear();
	grid.clear();
	grid = std::vector(image.getSize().x, std::vector(image.getSize().y, (sf::Texture*)nullptr));

	sf::Vector2f playerPosition;


	for (size_t x = 0; x < grid.size(); x++)
	{
		for (size_t y = 0; y < grid[x].size(); y++)
		{
			sf::Color pixelColor = image.getPixel(x, y);
			GameObject* object = nullptr;

			if (pixelColor == sf::Color::Red)
			{
				playerPosition = sf::Vector2f(x * cellSize + cellSize / 2.0f, y * cellSize + cellSize / 2.0f);
				continue;
			}

			else if (pixelColor == sf::Color::Black)
			{
				grid[x][y] = &Resources::textures["levelBrick.png"];

			}
			
			else if (pixelColor == sf::Color::Yellow)
			{
				object = new Coin();
				
			}

			else if (pixelColor == sf::Color::Blue)
			{
				object = new Goomba();

			}

			else if (pixelColor == sf::Color::Cyan)
			{
				object = new BulletBill();

			}

			else if (pixelColor == sf::Color::Magenta)
			{
				object = new PiranhaPlant();

			}

			else if (pixelColor == sf::Color::Green)
			{
				object = new Star();

			}

			if (object)
			{
				object->position = sf::Vector2f(x * cellSize + cellSize / 2.0f, y * cellSize + cellSize / 2.0f);
				objects.push_back(object);
			}
			else if (grid[x][y])
			{

				b2BodyDef bodyDef{};
				bodyDef.position.Set(x * cellSize + cellSize / 2.0f, y * cellSize + cellSize / 2.0f);
				b2Body* body = PhysicsEngine::world->CreateBody(&bodyDef);

				b2PolygonShape shape{};
				shape.SetAsBox(cellSize / 2.0f, cellSize / 2.0f);

				FixtureData* fixtureData = new FixtureData();
				fixtureData->type = FixtureDataType::MapTile;
				fixtureData->levelX = x;
				fixtureData->levelY = y;

				b2FixtureDef fixtureDef{};
				fixtureDef.userData.pointer = (uintptr_t)fixtureData;
				fixtureDef.density = 0.0f;
				fixtureDef.shape = &shape;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	return playerPosition;
}

void Level::Render(TextureController& textureController)
{
	int x = 0;
	for (const auto& column : grid)
	{
		int y = 0;
		for (const auto& cell : column)
		{
			if (cell)
			{
				textureController.Draw(*cell,sf::Vector2f(cellSize * x + cellSize / 2.0f, cellSize * y + cellSize / 2.0f), 
					sf::Vector2f(cellSize, cellSize));
			}
			y++;
		}
		x++;
	}

}
