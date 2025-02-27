#include "BulletBill.h"
#include "GameController.h"
#include "Resources.h"
#include "PhysicsEngine.h"
#include <box2d/b2_circle_shape.h>

void BulletBill::Initialize()
{

	objectTag = "bullet";

	//movementSpeed = -1.0f;

	bulletBillAnimation = Animation(0.5f,
		{
			AnimationFrame(0.00f, Resources::textures["bulletBill.png"]),
		});

	collisionData.object = this;
	collisionData.type = FixtureDataType::Object;

	b2BodyDef bodyDef{};
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	bodyDef.gravityScale = 0.0f;
	bulletBillBody = PhysicsEngine::world->CreateBody(&bodyDef);

	b2CircleShape circleShape{};
	circleShape.m_radius = 0.5f;

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&collisionData;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	bulletBillBody->CreateFixture(&fixtureDef);
}

void BulletBill::UpdateGameObject(float deltaTime)
{

	isDead = false;
	bulletBillAnimation.UpdateAnimation(deltaTime);

	b2Vec2 velocity = bulletBillBody->GetLinearVelocity();

	if (velocity.x < 0.02f)
		isFacingLeft = true;
	else if (velocity.x > -0.02f)
		isFacingLeft = false;

	if (std::abs(velocity.x) <= 0.02f)
		movement *= -1.0f;

	velocity.x = movement;

	bulletBillBody->SetLinearVelocity(velocity);

	position = sf::Vector2f(bulletBillBody->GetPosition().x, bulletBillBody->GetPosition().y);
	rotationAngle = bulletBillBody->GetAngle() * (180.0f / M_PI);
}
void BulletBill::RenderGameObject(TextureController& renderer)
{
	renderer.Draw(bulletBillAnimation.GetCurrentTexture(), !isDead ? position : sf::Vector2f(position.x,
		position.y + 0.45f), sf::Vector2f(isFacingLeft ? -1.0f : 1.0f, isDead ? 0.2f : 1.0f), rotationAngle);
}

bool BulletBill::IsDead()
{
	return isDead;
}
