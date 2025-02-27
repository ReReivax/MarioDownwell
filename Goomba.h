#pragma once

#include "Animation.h"
#include "GameObject.h"
#include "PhysicsEngine.h"

class Goomba : public GameObject
{
public: 
	virtual void Initialize() override;
	virtual void UpdateGameObject(float deltaTime) override;
	virtual void RenderGameObject(TextureController& textureController) override;

	void Die();
	bool IsDead();

private:

	FixtureData fixtureData;
	b2Body* body;
	Animation animation;

	float movement = 5.0f;
	float destroyGameObjectTimer = 0.0f;

	bool isDead = false;

};

