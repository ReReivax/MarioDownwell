#include "Goomba.h"
#include "GameController.h"
#include "Resources.h"
#include "PhysicsEngine.h"
#include <box2d/b2_circle_shape.h>

void Goomba::Initialize()
{
	animation = Animation(0.5f,
		{
			AnimationFrame(0.25f, Resources::textures["goomba2.png"]),
			AnimationFrame(0.00f, Resources::textures["goomba1.png"]),
		});

	objectTag = "goomba";

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

void Goomba::UpdateGameObject(float deltaTime)
{
	if (isDead)
	{
		destroyGameObjectTimer += deltaTime;
		if (destroyGameObjectTimer >= 2.0f)
			DeleteGameObject(this);

		return;
	}
	animation.UpdateAnimation(deltaTime);

	b2Vec2 velocity = body->GetLinearVelocity();

	if (std::abs(velocity.x) <= 0.02f)
		movement *= -1.0f;

	velocity.x = movement;

	body->SetLinearVelocity(velocity);

	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
	rotationAngle = body->GetAngle() * (180.0f / M_PI);
}

void Goomba::RenderGameObject(TextureController& renderer)
{
	bool flipTexture = movement < 0;

	renderer.Draw(animation.GetCurrentTexture(), !isDead ? position : sf::Vector2f(position.x,
		position.y + 0.45f), sf::Vector2f(flipTexture ? -1.0f : 1.0f, isDead ? 0.2f : 1.0f), rotationAngle);
}

void Goomba::Die() {
	isDead = true;
	destroyGameObjectTimer = 0.0f; // Reset the destroy timer
	PhysicsEngine::bodiesToDestroy.push_back(body);
}

bool Goomba::IsDead()
{
	return isDead;
}
