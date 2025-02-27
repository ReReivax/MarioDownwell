#pragma once
#include <SFML/Graphics.hpp>

class TextureController
{
public:
	TextureController(sf::RenderTarget& renderTarget);

	void Draw(const sf::Texture& texture, 
		const sf::Vector2f& position,const sf::Vector2f& size, float angle = 0.0f);
	
	sf::RenderTarget& renderTarget;

private:
	sf::Sprite sprite;
};

