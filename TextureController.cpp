#include "TextureController.h"
#include <vector>

TextureController::TextureController(sf::RenderTarget& renderTarget)
	: renderTarget(renderTarget)
{}

void TextureController::Draw(const sf::Texture& texture, const sf::Vector2f& position,
	const sf::Vector2f& size, float angle)
{
	sprite.setTexture(texture, true);
	sprite.setOrigin((sf::Vector2f)texture.getSize() / 2.0f);
	sprite.setRotation(angle);
	sprite.setPosition(position);
	sprite.setScale(sf::Vector2f(size.x / texture.getSize().x, size.y / texture.getSize().y));
	
	renderTarget.draw(sprite);
}
