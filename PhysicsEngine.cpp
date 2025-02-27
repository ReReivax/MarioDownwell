#include "PhysicsEngine.h"
#include <SFML/Graphics.hpp>
#include <box2d/b2_contact.h>
#include <box2d/b2_draw.h>
#include <box2d/b2_world_callbacks.h>

b2World* PhysicsEngine::world = nullptr;
std::vector<b2Body*> PhysicsEngine::bodiesToDestroy;

class MyGlobalContactListener :
	public b2ContactListener
{
	virtual void BeginContact(b2Contact* contact) override
	{
		FixtureData* data = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;

		if (data && data->listener)
			data->listener->OnBeginContact(contact->GetFixtureA(),
				contact->GetFixtureB());

		data = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;

		if (data && data->listener)
			data->listener->OnBeginContact(contact->GetFixtureB(),
				contact->GetFixtureA());
	}

	virtual void EndContact(b2Contact* contact) override
	{
		FixtureData* data = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;

		if (data && data->listener)
			data->listener->OnEndContact(contact->GetFixtureA(),
				contact->GetFixtureB());

		data = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;

		if (data && data->listener)
			data->listener->OnEndContact(contact->GetFixtureB(),
				contact->GetFixtureA());
	}
};

void PhysicsEngine::Initialize() {
	if (!world) {
		world = new b2World(b2Vec2(0.0f, 9.2f));
		world->SetContactListener(new MyGlobalContactListener());
	}
}

void PhysicsEngine::Cleanup() {
	if (world) {
		delete world;
		world = nullptr;
	}
}

void PhysicsEngine::Update(float deltaTime) {
	if (world) {
		world->Step(deltaTime, 8, 3);
		for (b2Body* body : bodiesToDestroy) {
			PhysicsEngine::world->DestroyBody(body);
		}
		bodiesToDestroy.clear();
	}
}
