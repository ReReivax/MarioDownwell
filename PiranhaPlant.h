#pragma once
#include "Animation.h"
#include "GameObject.h"
#include "PhysicsEngine.h"

class PiranhaPlant : public GameObject
{
public:
	virtual void Initialize() override;
	virtual void UpdateGameObject(float deltaTime) override;
	virtual void RenderGameObject(TextureController& textureController) override;

	bool IsDead();

private:

	FixtureData fixtureData;
	b2Body* body;

	Animation animation;

	float destroyTimer = 0.0f;

	bool isDead = false;
	bool isFacingLeft = false;

};

