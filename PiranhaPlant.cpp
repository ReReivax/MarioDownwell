#include "PiranhaPlant.h"
#include "GameController.h"
#include "Resources.h"
#include "PhysicsEngine.h"
#include <box2d/b2_circle_shape.h>

void PiranhaPlant::Initialize()
{
	animation = Animation(0.5f,
		{
			AnimationFrame(0.25f, Resources::textures["piranhaplant02.png"]),
			AnimationFrame(0.00f, Resources::textures["piranhaplant01.png"])
		});

	objectTag = "plant";

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

void PiranhaPlant::UpdateGameObject(float deltaTime)
{

	isDead = false;
	animation.UpdateAnimation(deltaTime);

	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
	rotationAngle = body->GetAngle() * (180.0f / M_PI);
}
void PiranhaPlant::RenderGameObject(TextureController& renderer)
{
	renderer.Draw(animation.GetCurrentTexture(), !isDead ? position : sf::Vector2f(position.x,
		position.y + 0.45f), sf::Vector2f(isFacingLeft ? -1.0f : 1.0f, isDead ? 0.2f : 1.0f), rotationAngle);
}

bool PiranhaPlant::IsDead()
{
	return isDead;
}
