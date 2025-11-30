#pragma once
#include "header.h"

extern SDL_Texture* Menu;
extern SDL_Texture* Hello;
extern SDL_Texture* Settings, * Help, * About, * Load, * PlayGame, * Save, * Pause, * PvP, * PvE;

void DrawBackGround();
SDL_Texture* loadTexture(const char* pFile);
void renderWinEffect(int winner);
void renderFloatingText(string text, int x, int y, SDL_Color color, TTF_Font* font);
void renderRainbowText(string text, int x, int y, TTF_Font* font);
void renderRainbowText(string text, int x, int y);
void renderScrollingText(string text, int y, SDL_Color color);
void renderTextWithShadow(string text, int x, int y, SDL_Color mainColor, TTF_Font* font);
void renderPulsingText(string text, int x, int y, SDL_Color color);
void renderGlitchText(string text, int x, int y, TTF_Font* font);
void renderShakingText(string text, int x, int y, SDL_Color color, TTF_Font* font);
