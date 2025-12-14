#include "header.h"
#include "img.h"
using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


Mix_Music* GameMusic = NULL;
Mix_Chunk* ClickSound = NULL;
Mix_Chunk* WinSound = NULL;
TTF_Font* _font1 = NULL, * _font2 = NULL, * _font3 = NULL, * _font4 = NULL;


bool gameRunning = true;
int winner = 0;


bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Tạm thời bỏ Audio để tránh lỗi nếu chưa setup
		cout << "SDL khong the khoi tao! " << SDL_GetError() << endl;
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		cout << "SDL_Image khoi tao that bai! " << IMG_GetError() << endl;
		return false;
	}
	if (TTF_Init() == -1) {
		cout << "SDL_TTF khoi tao that bai! " << TTF_GetError() << endl;
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! Mixer_Error: %s\n", Mix_GetError());
		return false;
	}
	window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (window == NULL) return false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) return false;

	return true;
}

bool loadMedia() {
	_font1 = TTF_OpenFont("font/ARCADE.ttf", 100);
	if (_font1 == NULL) {
		cout << "Khong load duoc font! Thu kiem tra lai duong dan." << TTF_GetError() << endl;
		return false;
	}
	_font2 = TTF_OpenFont("font/ARCADE.ttf", 70);
	if (_font2 == NULL) {
		cout << "Khong load duoc font! Thu kiem tra lai duong dan." << TTF_GetError() << endl;
		return false;
	}
	_font3 = TTF_OpenFont("font/ARCADE.ttf", 35);
	if (_font3 == NULL) {
		cout << "Khong load duoc font! Thu kiem tra lai duong dan." << TTF_GetError() << endl;
		return false;
	}
	_font4 = TTF_OpenFont("font/ARCADE.ttf", 40);
	if (_font4 == NULL) {
		cout << "Khong load duoc font! Thu kiem tra lai duong dan." << TTF_GetError() << endl;
		return false;
	}
	GameMusic = Mix_LoadMUS("sound/MenuMusic_2.mp3");
	if (GameMusic == NULL) {
		cout << "Khong load duoc nhac game! Thu kiem tra lai duong dan. " << Mix_GetError() << endl;
		return false;
	}
	ClickSound = Mix_LoadWAV("sound/click_4.wav");
	if (ClickSound == NULL) {
		cout << "Khong the load duoc tieng click game!" << Mix_GetError() << endl;
		return false;
	}
	WinSound = Mix_LoadWAV("sound/winningSound.mp3");
	if (WinSound == NULL) {
		cout << "Khong the load tieng win!" << Mix_GetError() << endl;
		return false;
	}
	Menu = loadTexture("img/Menu.png");
	if (Menu == NULL) {
		cout << "tai Menu that bai!" << IMG_GetError() << endl;
		return false;
	}
	Help = loadTexture("img/Help.png");
	if (Help == NULL) {
		cout << "Tai Help that bai!" << IMG_GetError() << endl;
		return false;
	}
	About = loadTexture("img/About.png");
	if (About == NULL) {
		cout << "Tai About that bai!" << IMG_GetError() << endl;
		return false;
	}
	Settings = loadTexture("img/Settings.png");
	if (Settings == NULL) {
		cout << "Tai Settings that bai!" << IMG_GetError() << endl;
		return false;
	}
	PvP = loadTexture("img/PvP.png");
	if (PvP == NULL) {
		cout << "Tai PvP that bai!" << IMG_GetError() << endl;
		return false;
	}
	Load = loadTexture("img/Load.png");
	if (Load == NULL) {
		cout << "Tai Load that bai!" << IMG_GetError() << endl;
		return false;
	}
	Hello = loadTexture("img/Hello.png");
	if (Hello == NULL) {
		cout << "Tai Hello That bai!" << IMG_GetError() << endl;
		return false;
	}
	Save = loadTexture("img/Save.png");
	if (Save == NULL) {
		cout << "Tai Save that bai!" << IMG_GetError() << endl;
		return false;
	}
	Pause = loadTexture("img/Pause.png");
	if (Pause == NULL) {
		cout << "Tai Pause that bai!" << IMG_GetError() << endl;
		return false;
	}
	PlayGame = loadTexture("img/GameMode.png");
	if (PlayGame == NULL) {
		cout << "Khong the tai PlayGame!" << IMG_GetError() << endl;
		return false;
	}
	PvE = loadTexture("img/PvE.png");
	if (PvE == NULL) {
		cout << "Tai anh PvE that bai!" << IMG_GetError();
		return false;
	}
	return true;
}

void closeSDL() {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	Mix_FreeMusic(GameMusic);
	Mix_FreeChunk(WinSound);
	Mix_FreeChunk(ClickSound);
	TTF_CloseFont(_font1);
	
	SDL_DestroyTexture(Menu);
	SDL_DestroyTexture(PlayGame);
	SDL_DestroyTexture(About);
	SDL_DestroyTexture(Help);
	SDL_DestroyTexture(Settings);
	SDL_DestroyTexture(Load);
	SDL_DestroyTexture(Hello);
	SDL_DestroyTexture(Save);
	SDL_DestroyTexture(Pause);
	SDL_DestroyTexture(PvP);
	SDL_DestroyTexture(PvE);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();

}

int main(int argc, char* args[]) {
	if (!init()) {
		cout << "Khoi tao that bai!" << endl;
		return -1;
	}
	if (!loadMedia()) {
		cout << "Load media that bai! (Kiem tra font)" << endl;
	}
	initSnow();
	initBoard();
	SDL_Event e;

	while (gameRunning) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				gameRunning = false;

			if (currentState == STATE_SAVE) {
				if (e.type == SDL_TEXTINPUT) {
					if (inputText.length() < 15) inputText += e.text.text;
				}
				else if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) inputText.pop_back();
					else if (e.key.keysym.sym == SDLK_ESCAPE) {
						SDL_StopTextInput();
						// Hủy save thì quay về game
						if (playGameSelection == 0) currentState = STATE_PvP;
						else if (playGameSelection == 1) currentState = STATE_PvE_easy;
						else currentState = STATE_PvE_hard;
					}
					else if (e.key.keysym.sym == SDLK_RETURN) {
						if (inputText.length() > 0) {
							SaveGame(inputText);
							// Save xong quay lại chơi
							if (playGameSelection == 0) currentState = STATE_PvP;
							else if (playGameSelection == 1) currentState = STATE_PvE_easy;
							else currentState = STATE_PvE_hard;
							SDL_StopTextInput();
						}
					}
				}
				
			}
			else if (e.type == SDL_KEYDOWN) {
				// TH1: MENU.
				if (currentState == STATE_MENU) {
					switch (e.key.keysym.sym) {
					case SDLK_UP: case SDLK_w:
						menuSelection--;
						if (menuSelection < 0)
							menuSelection = 5;
						break;
					case SDLK_DOWN: case SDLK_s:
						menuSelection++;
						if (menuSelection > 5)
							menuSelection = 0;
						break;
					case SDLK_ESCAPE:
						currentState = STATE_HELLO;
						break;
					case SDLK_RETURN: case SDLK_SPACE: case SDLK_KP_ENTER:
						if (menuSelection == 0) {
							currentState = STATE_PLAYING;
						}
						else if (menuSelection == 1) {
							scanSaveFiles();
							currentState = STATE_LOAD;

						}

						else if (menuSelection == 2)
							currentState = STATE_ABOUT;

						else if (menuSelection == 3)
							currentState = STATE_HELP;

						else if (menuSelection == 4)
							currentState = STATE_SETTING;

						else if (menuSelection == 5)
							gameRunning = false;

						break;
					}
				}
				// TH2: PvP
				else if (currentState == STATE_PvP) {
					if (winner == 0) {
						if (e.key.keysym.sym == SDLK_ESCAPE) {
							currentState = STATE_PAUSE;
						}
						else if (e.key.keysym.sym == SDLK_l) {
							currentState = STATE_SAVE;
							inputText = "";
							SDL_StartTextInput();
						}
						else if (currentPlayer == 1) {
							switch (e.key.keysym.sym) {
							case SDLK_w:
								cursorRow--;
								if (cursorRow < 0) cursorRow = BOARD_SIZE - 1;
								break;
							case SDLK_s:
								cursorRow++;
								if (cursorRow > 11) cursorRow = 0;
								break;
							case SDLK_a:
								cursorCol--;
								if (cursorCol < 0) cursorCol = BOARD_SIZE - 1;
								break;
							case SDLK_d:
								cursorCol++;
								if (cursorCol > 11) cursorCol = 0;
								break;
							case SDLK_SPACE:
								if (placeMove(cursorRow, cursorCol)) {
									if (ClickSound != NULL && cSound == true) {
										Mix_PlayChannel(-1, ClickSound, 0);
									}
									int res = checkWin(cursorRow, cursorCol);
									if (res != 0) {
										if (WinSound != NULL && cSound == true) {
											Mix_PlayChannel(-1, WinSound, 0);
										}
										winner = res;
									}
									else if (checkDraw()) {
										winner = 3;
									}
								}
								break;
							}
						}
						else if (currentPlayer == 2) {
							switch (e.key.keysym.sym) {
							case SDLK_UP:
								cursorRow--;
								if (cursorRow < 0) cursorRow = BOARD_SIZE - 1;
								break;
							case SDLK_DOWN:
								cursorRow++;
								if (cursorRow > 11) cursorRow = 0;
								break;
							case SDLK_LEFT:
								cursorCol--;
								if (cursorCol < 0) cursorCol = BOARD_SIZE - 1;
								break;
							case SDLK_RIGHT:
								cursorCol++;
								if (cursorCol > 11) cursorCol = 0;
								break;
							case SDLK_RETURN: case SDLK_KP_ENTER:
								if (placeMove(cursorRow, cursorCol)) {
									if (ClickSound != NULL && cSound == true) {
										Mix_PlayChannel(-1, ClickSound, 0);
									}
									int res = checkWin(cursorRow, cursorCol);
									if (res != 0) {
										if (WinSound != NULL && cSound == true) {
											Mix_PlayChannel(-1, WinSound, 0);
										}
										winner = res;
									}
									else if (checkDraw()) {
										winner = 3;
									}
								}
								break;
							}
						}
					}

					else {
						if (e.key.keysym.sym == SDLK_y) {
							currentState = STATE_PvP;
							if (winner == 1) {
								countWinP1++;
							}
							else if (winner == 2) {
								countWinP2++;
							}
							winner = 0;
							initBoard();

						}
						else if (e.key.keysym.sym == SDLK_n) {
							currentState = STATE_MENU;
							countWinP1 = 0;
							countWinP2 = 0;
							winner = 0;
							initBoard();
						}
					}
				}
				// TH3: ABOUT
				else if (currentState == STATE_ABOUT) {
					if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_SPACE)
						currentState = STATE_MENU;
				}
				// TH4: LOAD
				else if (currentState == STATE_LOAD) {
					switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						currentState = STATE_MENU;
						break;
					case SDLK_w:
					case SDLK_UP:
						loadSelection--;
						if (loadSelection < 0) loadSelection = saveFiles.size() - 1;
						break;
					case SDLK_s:
					case SDLK_DOWN:
						loadSelection++;
						if (loadSelection > saveFiles.size() - 1) loadSelection = 0;
						break;
					case SDLK_RETURN:
					case SDLK_SPACE:
						if(saveFiles.size() != 0)
						{
							string selectedFile = saveFiles[loadSelection];
							if (LoadGame(selectedFile)) {
								winner = 0;

								lastTime = SDL_GetTicks();
							}
							else {
								if (e.key.keysym.sym == SDLK_ESCAPE)
									currentState = STATE_LOAD;
							}
							break;
						}
						else {
							currentState = STATE_MENU;
						}
					}
				}
				// TH5: HELP
				else if (currentState == STATE_HELP) {
					if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_SPACE) {
						if (!isPause)
							currentState = STATE_MENU;
						else
							currentState = STATE_PAUSE;
					}
				}
				//TH6: SETTING
				else if (currentState == STATE_SETTING) {
					switch (e.key.keysym.sym) {
					case SDLK_w: case SDLK_UP:
						settingSelection--;
						if (settingSelection < 0)
							settingSelection = 2;
						break;
					case SDLK_s: case SDLK_DOWN:
						settingSelection++;
						if (settingSelection > 2)
							settingSelection = 0;
						break;
					case SDLK_RETURN:
						if (settingSelection == 0) {
							if (mainMusic) {
								mainMusic = false;
							}
							else {
								mainMusic = true;
							}
						}
						else if (settingSelection == 1) {
							if (cSound)
								cSound = false;
							else
								cSound = true;
						}
						else if (settingSelection == 2)
							if (effects) effects = false;
							else effects = true;
						break;
					case SDLK_ESCAPE:
						if (isPause)
							currentState = STATE_PAUSE;
						else
							currentState = STATE_MENU;
						break;
					}

				}
				// TH7: HELLO
				else if (currentState == STATE_HELLO) {
					if (e.key.keysym.sym != SDLK_ESCAPE) {
						currentState = STATE_MENU;
					}
				}
				// TH8: SAVE GAME
				else if (currentState == STATE_SAVE) {

				}
				// TH9: PAUSE GAME
				else if (currentState == STATE_PAUSE) {
					isPause = true;
					switch (e.key.keysym.sym) {
					case SDLK_s: case SDLK_DOWN:
						pauseSelection++;
						if (pauseSelection > 3)
							pauseSelection = 0;
						break;
					case SDLK_w: case SDLK_UP:
						pauseSelection--;
						if (pauseSelection < 0)
							pauseSelection = 3;
						break;
					case SDLK_RETURN: case SDLK_SPACE: case SDLK_KP_ENTER:
						if (pauseSelection == 0) {
							if (playGameSelection == 0)
								currentState = STATE_PvP;
							else if (playGameSelection == 1)
								currentState = STATE_PvE_easy;
							else if (playGameSelection == 2)
								currentState = STATE_PvE_hard;
							isPause = false;
						}
						else if (pauseSelection == 1)
							currentState = STATE_SETTING;
						else if (pauseSelection == 2)
							currentState = STATE_HELP;
						else if (pauseSelection == 3) {
							currentState = STATE_MENU;
							isPause = false;
						}
						break;
					}
				}
				//TH10: PLAY GAME
				else if (currentState == STATE_PLAYING) {
					switch (e.key.keysym.sym) {
					case SDLK_UP: case SDLK_w:
						playGameSelection--;
						if (playGameSelection < 0)
							playGameSelection = 3;
						break;
					case SDLK_DOWN: case SDLK_s:
						playGameSelection++;
						if (playGameSelection > 3)
							playGameSelection = 0;
						break;
					case SDLK_RETURN:
					case SDLK_SPACE:
						if (playGameSelection == 0) {
							currentState = STATE_PvP;
							initBoard();
							winner = 0;
						}
						else if (playGameSelection == 1)
						{
							currentState = STATE_PvE_easy;
							initBoard();
							winner = 0;
						}
							
						else if (playGameSelection == 2)
						{
							currentState = STATE_PvE_medium;
							initBoard();
							winner = 0;
						}
						else if (playGameSelection == 3)
						{
							currentState = STATE_PvE_hard;
							initBoard();
							winner = 0;
						}
						break;
					case SDLK_ESCAPE:
						currentState = STATE_MENU;
						break;
					}
				}
				// TH11: PvE
				else if (currentState == STATE_PvE_easy || currentState == STATE_PvE_hard || currentState == STATE_PvE_medium) {
					if (winner == 0) {
						if (e.key.keysym.sym == SDLK_ESCAPE)
							currentState = STATE_PAUSE;
						else if (e.key.keysym.sym == SDLK_l)
						{
							currentState = STATE_SAVE;
							inputText = "";
							SDL_StartTextInput();
						}
						else if (currentPlayer == 1) {
							switch (e.key.keysym.sym) {
							case SDLK_w:
								cursorRow--;
								if (cursorRow < 0) cursorRow = BOARD_SIZE - 1;
								break;
							case SDLK_s:
								cursorRow++;
								if (cursorRow > BOARD_SIZE - 1) cursorRow = 0;
								break;
							case SDLK_a:
								cursorCol--;
								if (cursorCol < 0) cursorCol = BOARD_SIZE - 1;
								break;
							case SDLK_d:
								cursorCol++;
								if (cursorCol > BOARD_SIZE - 1) cursorCol = 0;
								break;
							case SDLK_RETURN:
							case SDLK_SPACE:
								if (placeMove(cursorRow, cursorCol)) {
									if (ClickSound != NULL && cSound) {
										Mix_PlayChannel(-1, ClickSound, 0);
									}
									int res = checkWin(cursorRow, cursorCol);
									if (res != 0) {
										if (WinSound != NULL && cSound) {
											Mix_PlayChannel(-1, WinSound, 0);
										}
										winner = res;
									}
									else if (checkDraw())
										winner = 3;
								}

							}
							break;
						}
					}
					else {
						if (e.key.keysym.sym == SDLK_y) {
							if (playGameSelection == 1)
								currentState = STATE_PvE_easy;
							else if (playGameSelection == 2)
								currentState = STATE_PvE_medium;
							else if (playGameSelection == 3)
								currentState = STATE_PvE_hard;
							if (winner == 1) {
								countWinP1++;
							}
							else if (winner == 2) {
								countWinP2++;
							}
							winner = 0;
							initBoard();
						}
						else if (e.key.keysym.sym == SDLK_n) {
							currentState = STATE_MENU;
							countWinP1 = 0;
							countWinP2 = 0;
							winner = 0;
							initBoard();
						}
					}
				}
			}
		}
		
		updateTimer();
		if (winner == 0 && currentPlayer == 2 && (currentState == STATE_PvE_easy || currentState == STATE_PvE_hard || currentState == STATE_PvE_medium)) {

			if (currentState == STATE_PvE_easy) {
				PVE_Easy_Move(); // Gọi Bot Dễ
			}
			else if(currentState == STATE_PvE_medium) {
				PVE_Hard_Move(); // Gọi Bot Khó
			}
			else {
				PVE_VeryHard_Move();
			}

			int res = checkWin(lastMoveX, lastMoveY);

			if (res != 0) {
				if (WinSound != NULL && cSound) Mix_PlayChannel(-1, WinSound, 0);
				winner = res;
			}
			else if (checkDraw()) {
				winner = 3;
			}
		}
		if (mainMusic) {
			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(GameMusic, -1);
			}
			else if (Mix_PausedMusic() == 1) {
				Mix_ResumeMusic();
			}
		}
		else {
			if (Mix_PlayingMusic() == 1) {
				Mix_HaltMusic();
			}
		}
		DrawBackGround();

		if(effects)
			renderSnowEffect();

		//Vẽ nền, chữ
		if (currentState == STATE_MENU) {
			DrawMenu();
			
		}
		else if (currentState == STATE_PvP || currentState == STATE_PvE_easy || currentState == STATE_PvE_hard || currentState == STATE_PvE_medium) {
			DrawBoard();
			DrawUI();
			DrawXO();
			if (winner != 0)
				renderWinEffect(winner);
		}
		else if (currentState == STATE_ABOUT) {
			renderAbout();
		}
		else if (currentState == STATE_HELP) {
			renderSnowEffect();
			renderHelp();
		}
		else if (currentState == STATE_SETTING) {
			renderSetting();
		}
		else if (currentState == STATE_LOAD) {
			renderLoad();
		}
		else if (currentState == STATE_SAVE) {
			DrawNameInput();
		}
		else if (currentState == STATE_HELLO) {
			renderHello();
		}
		else if (currentState == STATE_PAUSE) {
			renderPause();
		}
		else if (currentState == STATE_PLAYING) {
			renderPlay();
		}
		SDL_RenderPresent(renderer);
	}
	closeSDL();
	return 0;
}