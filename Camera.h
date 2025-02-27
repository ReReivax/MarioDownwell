#pragma once
#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(float initial_zoom_level = 5.0f);
    sf::Vector2f GetViewSize();
    sf::View GetCameraView(sf::Vector2u window_size);
    sf::View GetUIView();

    float zoomLevel;
    sf::Vector2f cameraPosition;

private:
    sf::Vector2f viewSize;
};


