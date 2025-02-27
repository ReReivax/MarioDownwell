#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureController.h"
#include "GameObject.h"

class Level
{
public:
	Level(float cellSize = 32.0f);

	sf::Vector2f CreateLevelByImage(const sf::Image& image, std::vector<GameObject*>& objects);
	void Render(TextureController& textureController);
	
	std::vector<std::vector<sf::Texture*>> grid;
	float cellSize;

};

