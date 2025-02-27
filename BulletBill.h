#pragma once

#include "Animation.h"
#include "GameObject.h"
#include "PhysicsEngine.h"


class BulletBill : public GameObject
{
public:
	float movementSpeed;

	virtual void Initialize() override;
	virtual void UpdateGameObject(float deltaTime) override;
	virtual void RenderGameObject(TextureController& textureController) override;

	bool IsDead();

private:

	FixtureData collisionData;
	b2Body* bulletBillBody;


	Animation bulletBillAnimation;

	float movement = 5.0f;
	float destroyObjectTimer = 0.0f;

	bool isDead = false;
	bool isFacingLeft = false;

};

