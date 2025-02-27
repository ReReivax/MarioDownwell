#pragma once
#include "TextureController.h"
#include "PhysicsEngine.h" 
#include "Animation.h"
#include "ContactListener.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player : public ContactListener
{
public:
    void InitializePlayer();
    void Update(float deltaTime);
    void RenderPlayer(TextureController& textureController);

    void OnBeginContact(b2Fixture* self, b2Fixture* other);
    void OnEndContact(b2Fixture* self, b2Fixture* other);

    b2Fixture* GetGroundFixture();

    size_t GetCoins();
    size_t GetJumpFuel();

    sf::Vector2f playerPosition;
    float playerRotationAngle;

    bool isDead;
    bool hasWon;

private:
    
    Animation runAnimation;

    sf::Texture textureToDraw;

    sf::Sound jumpSoundEffect;
    sf::Sound coinCollectedSoundEffect;
    sf::Sound squishSoundEffect;
    sf::Sound gameOverSoundEffect;
    sf::Sound winSoundEffect;

    FixtureData fixtureData;
    b2Body* body;
    b2Fixture* groundCollisionFixture;

    size_t isGrounded;
    size_t coins;

    bool facingLeft = false;
};

