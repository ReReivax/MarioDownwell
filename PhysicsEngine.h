#pragma once
#include "TextureController.h"
#include "ContactListener.h"
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>

class Player;
class GameObject;

enum class FixtureDataType
{
	Player,
	MapTile,
	Object
};

struct FixtureData
{
	ContactListener* listener;
	FixtureDataType type;
	
	union
	{
		Player* player;
		GameObject* object;
		struct { int levelX, levelY; };
	};
};

class PhysicsEngine
{
public:
	static void Initialize();
	static void Cleanup();
	static void Update(float deltaTime);
	static std::vector<b2Body*> bodiesToDestroy;
	static b2World* world;
};

