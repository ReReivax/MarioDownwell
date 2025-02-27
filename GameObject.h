#pragma once
#include "TextureController.h"

class GameObject
{
public:
	virtual void Initialize() {}
	virtual void UpdateGameObject(float deltaTime) {}
	virtual void RenderGameObject(TextureController& textureController) {}

	std::string objectTag;
	
	sf::Vector2f position;
	float rotationAngle;
};
