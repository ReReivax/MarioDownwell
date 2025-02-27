#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "PhysicsEngine.h"
#include "Timer.h"
#include <box2d/box2d.h>

class Coin : public GameObject
{
public:
    ~Coin();

    virtual void Initialize() override;
    virtual void UpdateGameObject(float elapsed_time) override;
    virtual void RenderGameObject(TextureController& texture_controller) override;

private:
    Animation coinAnimation;
    b2Body* coinBody;
};

