#pragma once
#include <SFML/Graphics.hpp>

struct AnimationFrame
{
    AnimationFrame(float frame_time = 0.0f, sf::Texture texture = sf::Texture())
        : frame_time(frame_time), texture(texture)
    {
    }

    sf::Texture texture;
    float frame_time;
};

class Animation
{
public:
    Animation(float animation_length = 0.0f, std::vector<AnimationFrame> animation_frames = {});
    void UpdateAnimation(float elapsed_time);

    sf::Texture GetCurrentTexture();

private:
    float animation_time = 0.0f;
    float length;
    std::vector<AnimationFrame> frames;
};