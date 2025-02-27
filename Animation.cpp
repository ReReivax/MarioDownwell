#include "Animation.h"

Animation::Animation(float animation_length, std::vector<AnimationFrame> animation_frames)
    : length(animation_length), frames(animation_frames)
{
}

void Animation::UpdateAnimation(float elapsed_time)
{
    animation_time += elapsed_time;
}

sf::Texture Animation::GetCurrentTexture()
{
    while (animation_time > length)
    {
        animation_time -= length;
    }

    for (auto& frame : frames)
    {
        if (animation_time >= frame.frame_time)
        {
            return frame.texture;
        }
    }

    return sf::Texture();
}