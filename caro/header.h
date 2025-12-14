#pragma once
#include <iostream>
#include <vector>
#include <string> 
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <windows.h>
#include <fstream>
#include <cmath>
#include <filesystem>

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 700
#define CELLSIZE 46
#define TOP 84
#define LEFT 363
#define BOARD_SIZE 12

enum GameState {
	STATE_HELLO,
	STATE_MENU,
	STATE_PLAYING,
	STATE_LOAD,
	STATE_ABOUT,
	STATE_HELP,
	STATE_SETTING,
	STATE_SAVE,
	STATE_PAUSE,
	STATE_PvP,
	STATE_PvE_easy,
	STATE_PvE_medium,
	STATE_PvE_hard
};
// --- GLOBAL VARIABLES ---
extern GameState currentState;
extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern TTF_Font* _font1, * _font2, * _font3, * _font4;
extern Mix_Music* GameMusic;
extern Mix_Chunk* ClickSound;
extern Mix_Chunk* WinSound;

extern int GameBoard[BOARD_SIZE][BOARD_SIZE];
extern int currentPlayer;
extern int winner;
extern bool gameRunning, isPause;
extern int lastMoveX, lastMoveY;
extern int cursorRow, cursorCol;
extern int menuSelection, playGameSelection;
extern bool mainMusic, cSound;
extern int settingSelection;
extern int pauseSelection;
extern int countMoveP1, countMoveP2;
extern int countWinP1, countWinP2;
extern string inputText;
extern int scrollX;
extern int turnTime;
extern Uint32 lastTime;
extern bool effects;

extern vector<string> saveFiles;
extern int loadSelection;

// --- MODEL ---
void initBoard();
bool placeMove(int x, int y);
int checkWin(int x, int y);
bool checkDraw();
void SaveGame(string pfile);
bool LoadGame(string pfile);
void updateTimer();
void scanSaveFiles();
void deletePFile(string filename);

// --- VIEW ---
struct SnowFlake {
	float x, y;     
	float speed;    
	int size;   
};

#define MAX_SNOW 100
extern SnowFlake snow[MAX_SNOW];
void renderSnowEffect();
void initSnow();
void renderText(string text, int x, int y, SDL_Color color);
void renderText(string text, int x, int y, SDL_Color color, TTF_Font* font);
void renderText(string text, int x, int y, SDL_Color color, TTF_Font* font, float scale);
void DrawBoard();
void DrawXO();
void DrawMenu();
void renderAbout();
void renderHelp();
void renderSetting();
void DrawUI();
void renderLoad();
void renderHello();
void DrawNameInput();
void renderPause();
void renderPlay();
void DrawLoadFileList();
SDL_Texture* createTextTexture(string text, SDL_Color color);

// --- BOT ---
void PVE_Easy_Move(); // Hàm bot dễ
void PVE_Hard_Move(); // Hàm bot khó
void PVE_VeryHard_Move();
// --- MAIN ---
bool init();
bool loadMedia();
void closeSDL();