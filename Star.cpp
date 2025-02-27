#include "Star.h"
#include "PiranhaPlant.h"
#include "GameController.h"
#include "Resources.h"
#include "PhysicsEngine.h"
#include <box2d/b2_circle_shape.h>

void Star::Initialize()
{
	animation = Animation(0.5f,
		{
			AnimationFrame(0.25f, Resources::textures["star04.png"]),
			AnimationFrame(0.00f, Resources::textures["star03.png"]),
			AnimationFrame(0.00f, Resources::textures["star02.png"]),
			AnimationFrame(0.00f, Resources::textures["star01.png"])
		});

	objectTag = "star";

	fixtureData.object = this;
	fixtureData.type = FixtureDataType::Object;

	b2BodyDef bodyDef{};
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = PhysicsEngine::world->CreateBody(&bodyDef);

	b2CircleShape circleShape{};
	circleShape.m_radius = 0.5f;

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
}

void Star::UpdateGameObject(float deltaTime)
{

	isDead = false;
	animation.UpdateAnimation(deltaTime);

	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
	rotationAngle = body->GetAngle() * (180.0f / M_PI);
}
void Star::RenderGameObject(TextureController& renderer)
{
	renderer.Draw(animation.GetCurrentTexture(), !isDead ? position : sf::Vector2f(position.x,
		position.y + 0.45f), sf::Vector2f(isFacingLeft ? -1.0f : 1.0f, isDead ? 0.2f : 1.0f), rotationAngle);
}

bool Star::IsDead()
{
	return isDead;
}