#pragma once
#include <SFML/Graphics.hpp>
#include "TextureController.h"
#include "GameObject.h"
#include "Camera.h"
#include "Timer.h"

extern Camera g_Camera;
extern Timer* g_GlobalTimer;
extern bool g_IsGamePaused;

void InitializeGame(); 
void Update(float deltaTime);
void RenderGameWindow(TextureController& textureController);
void RenderGameUI(TextureController& textureController);
void LoadResources();
void DeleteGameObject(GameObject* object);

static enum GameState
{
	MAIN_MENU,
	PLAYING,
	GAME_OVER
} gamestate;

const float M_PI = 3.14159265358979323846f;
