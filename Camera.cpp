#include "Camera.h"

Camera::Camera(float initial_zoom_level)
    : zoomLevel(initial_zoom_level)
{}

sf::Vector2f Camera::GetViewSize()
{
    return viewSize;
}

sf::View Camera::GetCameraView(sf::Vector2u window_size)
{
    float aspect_ratio = (float)window_size.x / (float)window_size.y;
    if (aspect_ratio > 1.0f)
    {
        viewSize.x = zoomLevel * aspect_ratio;
        viewSize.y = zoomLevel;
    }
    else
    {
        viewSize.x = zoomLevel;
        viewSize.y = zoomLevel / aspect_ratio;
    }

    return sf::View(cameraPosition, viewSize);
}

sf::View Camera::GetUIView()
{
    float aspect_ratio = (float)viewSize.x / (float)viewSize.y;
    viewSize = sf::Vector2f(100.0f, 100.0f / aspect_ratio);
    return sf::View(sf::Vector2f(), viewSize);
}