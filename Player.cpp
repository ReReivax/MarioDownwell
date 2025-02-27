#include "Player.h"
#include "Resources.h"
#include "GameObject.h"
#include "GameController.h"
#include "Goomba.h"
#include "BulletBill.h"
#include "PiranhaPlant.h"
#include "Star.h"
#include <iostream>

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>


constexpr float Math_PI = 22.0f / 7.0f;

const float movementSpeed = 7.0f;
const float jumpVelocity = 6.0f;

float destroyTimer = 0.0f;
int jumpFuel = 21;

void Player::InitializePlayer()
{
	hasWon = false;
	runAnimation = Animation(0.45f,
		{
		AnimationFrame(0.30f, Resources::textures["marioRun3.png"]),
		AnimationFrame(0.15f, Resources::textures["marioRun2.png"]),
		AnimationFrame(0.0f, Resources::textures["marioRun1.png"])
		});

	jumpSoundEffect.setBuffer(Resources::audio["jump.wav"]);
	jumpSoundEffect.setVolume(10.0f);

	coinCollectedSoundEffect.setBuffer(Resources::audio["Coin.wav"]);
	coinCollectedSoundEffect.setVolume(90.0f);

	squishSoundEffect.setBuffer(Resources::audio["Squish.wav"]);
	squishSoundEffect.setVolume(90.0f);

	gameOverSoundEffect.setBuffer(Resources::audio["Game Over.wav"]);
	gameOverSoundEffect.setVolume(90.0f);

	winSoundEffect.setBuffer(Resources::audio["smb_stage_clear.wav"]);
	winSoundEffect.setVolume(90.0f);


	fixtureData.listener = this;
	fixtureData.player = this;
	fixtureData.type = FixtureDataType::Player;

	b2BodyDef bodyDef{};
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(playerPosition.x, playerPosition.y);
	bodyDef.fixedRotation = true;
	body = PhysicsEngine::world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;

	b2CircleShape circleShape{};
	circleShape.m_radius = 0.5f;
	circleShape.m_p.Set(0.0f, -0.5f);
	fixtureDef.shape = &circleShape;
	body->CreateFixture(&fixtureDef);

	circleShape.m_p.Set(0.0f, 0.5f);
	body->CreateFixture(&fixtureDef);

	b2PolygonShape polygonShape{};
	polygonShape.SetAsBox(0.5f, 0.5f);
	fixtureDef.shape = &polygonShape;
	body->CreateFixture(&fixtureDef);

	polygonShape.SetAsBox(0.4f, 0.1f, b2Vec2(0.0f, 1.0f), 0.0f);
	fixtureDef.isSensor = true;
	groundCollisionFixture = body->CreateFixture(&fixtureDef);
}

void Player::Update(float deltaTime)
{
	float move = movementSpeed;
	const float maxFallingSpeed = 12.0f;
	bool isBoosting = false;

	runAnimation.UpdateAnimation(deltaTime);

	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x *= 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		velocity.x += move;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		velocity.x -= move;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isGrounded > 0)
	{
		velocity.y = -jumpVelocity;
		jumpSoundEffect.play();
		isGrounded = false;
		isBoosting = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && jumpFuel > 0)
	{

		velocity.y = -jumpVelocity;
		jumpSoundEffect.play();
		isGrounded = false;
		jumpFuel--;
		isBoosting = true;
	}

	if (!isGrounded)
	{
		if (velocity.y > maxFallingSpeed)
			velocity.y = maxFallingSpeed;
	}

	textureToDraw = runAnimation.GetCurrentTexture();

	if (velocity.x < -0.02f)
		facingLeft = true;
	else if (velocity.x > 0.02f)
		facingLeft = false;
	else
		textureToDraw = Resources::textures["marioIdle.png"];

	if (!isGrounded && !isBoosting)
		textureToDraw = Resources::textures["marioJump.png"];

	if(isBoosting)
		textureToDraw = Resources::textures["marioJumpBoost.png"];

	body->SetLinearVelocity(velocity);

	playerPosition = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
	playerRotationAngle = body->GetAngle() * (180.0f / Math_PI);

}

void Player::RenderPlayer(TextureController& textureController)
{
	textureController.Draw(textureToDraw, playerPosition,
		sf::Vector2f(facingLeft ? -1.0f : 1.0f, 2.0f), playerRotationAngle);
}

void Player::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* data = (FixtureData*)other->GetUserData().pointer;

	if (!data)
		return;

	if (groundCollisionFixture == self && data->type == FixtureDataType::MapTile)
	{
		isGrounded++;
		if (jumpFuel == 0)
		jumpFuel += 25;
	}

	else if (data && data->type == FixtureDataType::Object && data->object->objectTag == "coin")
	{
		coinCollectedSoundEffect.play();
		coins++;

		g_GlobalTimer->AddTime(5);
		jumpFuel += 5;
	
		DeleteGameObject(data->object);

	}

	else if (data->type == FixtureDataType::Object && data->object->objectTag == "goomba")
	{
		Goomba* enemy = dynamic_cast<Goomba*>(data->object);

		if (!enemy)
			return;

		if (groundCollisionFixture == self)
		{
			squishSoundEffect.play();
			enemy->Die();
		}
		else if (!enemy->IsDead())
		{
			isDead = true;
			jumpFuel = 0;
			gameOverSoundEffect.play();
		}
	}

	else if (data->type == FixtureDataType::Object && data->object->objectTag == "bullet")
	{
		BulletBill* enemy = dynamic_cast<BulletBill*>(data->object);
		if (!enemy)
			return;

		else if (!enemy->IsDead())
		{
			isDead = true;
			gameOverSoundEffect.play();
			jumpFuel = 0;
		}
	}

	else if (data->type == FixtureDataType::Object && data->object->objectTag == "plant")
	{
		PiranhaPlant* enemy = dynamic_cast<PiranhaPlant*>(data->object);
		if (!enemy)
			return;

		else if (!enemy->IsDead())
		{
			isDead = true;
			gameOverSoundEffect.play();
			jumpFuel = 0;
		}
	}

	else if (data->type == FixtureDataType::Object && data->object->objectTag == "star")
	{
		Star* enemy = dynamic_cast<Star*>(data->object);
		if (!enemy)
			return;

		else if (!enemy->IsDead())
		{
			isDead = true;
			hasWon = true;
			winSoundEffect.play();
			jumpFuel = 0;
		}
	}
}

void Player::OnEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* data = (FixtureData*)other->GetUserData().pointer;

	if (!data)
		return;

	if (groundCollisionFixture == self && data->type == FixtureDataType::MapTile && isGrounded > 0)
	{
		isGrounded--;
	}


}

size_t Player::GetCoins()
{
	return coins;
}

size_t Player::GetJumpFuel()
{
	return jumpFuel;
}

b2Fixture* Player::GetGroundFixture()
{
	return groundCollisionFixture;
}
