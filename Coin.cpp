#include "Coin.h"
#include "Resources.h"
#include <box2d/b2_fixture.h>
#include <box2d/b2_shape.h>
#include <box2d/b2_polygon_shape.h>

Coin::~Coin()
{
	PhysicsEngine::world->DestroyBody(coinBody);
}

void Coin::Initialize()
{
	objectTag = "coin";

	coinAnimation = Animation(1.4,
		{
			AnimationFrame(0.4f, Resources::textures["coin_0.png"]),
			AnimationFrame(0.3f, Resources::textures["coin_4.png"]),
			AnimationFrame(0.2f, Resources::textures["coin_3.png"]),
			AnimationFrame(0.1f, Resources::textures["coin_2.png"]),
			AnimationFrame(0.0f, Resources::textures["coin_1.png"]),
			
		});

	b2BodyDef bodyDef;
	bodyDef.position.Set(position.x, position.y);
	b2Body* coinBody = PhysicsEngine::world->CreateBody(&bodyDef);

	b2PolygonShape physicsShape{};
	physicsShape.SetAsBox(0.4f, 0.4f);

	FixtureData* fixtureData = new FixtureData();
	fixtureData->type = FixtureDataType::Object;
	fixtureData->object = this;	

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)fixtureData;
	fixtureDef.isSensor = true;
	fixtureDef.density = 0.0f;
	fixtureDef.shape = &physicsShape;
	coinBody->CreateFixture(&fixtureDef);

}

void Coin::UpdateGameObject(float deltaTime)
{
	coinAnimation.UpdateAnimation(deltaTime);
}

void Coin::RenderGameObject(TextureController& textureController)
{
	textureController.Draw(coinAnimation.GetCurrentTexture(), position, sf::Vector2f(0.8f, 0.8f));
}	