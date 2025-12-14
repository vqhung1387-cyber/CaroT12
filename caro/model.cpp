#include "header.h"
#include <filesystem>

using namespace std;
int GameBoard[BOARD_SIZE][BOARD_SIZE]; 
int currentPlayer;
int lastMoveX = -1, lastMoveY = -1;
int cursorRow = BOARD_SIZE / 2, cursorCol = BOARD_SIZE / 2;
int menuSelection = 0;
int settingSelection = 0;
int countMoveP1 = 0, countMoveP2 = 0;
int countWinP1 = 0, countWinP2 = 0;
GameState currentState = STATE_HELLO;
string inputText = "";
int pauseSelection = 0, playGameSelection;
bool isPause = false;
int turnTime = 15;
Uint32 lastTime = 0;
vector<string> saveFiles;
int loadSelection = 0;
SnowFlake snow[MAX_SNOW];
bool effects = true;

void initBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			GameBoard[i][j] = 0;
		}
	}
	if (!isPause) {
		currentPlayer = 1;
		countMoveP1 = 0;
		countMoveP2 = 0;
		lastMoveX = -1;
		lastMoveY = -1;
		cursorRow = BOARD_SIZE / 2;
		cursorCol = BOARD_SIZE / 2;
		turnTime = 15;
		lastTime = SDL_GetTicks();
	}
}

bool placeMove(int hang, int cot) { // Đổi tên biến thành hang, cot cho dễ hiểu
	// Kiểm tra biên
	if (hang < 0 || hang >= BOARD_SIZE || cot < 0 || cot >= BOARD_SIZE) {
		return false;
	}
	if (GameBoard[hang][cot] == 0) { // chưa ai đánh
		if (currentPlayer == 1) {
			GameBoard[hang][cot] = 1;
			countMoveP1++;
			currentPlayer = 2;
		}
		else {
			GameBoard[hang][cot] = 2;
			countMoveP2++;
			currentPlayer = 1;
		}
		lastMoveX = hang;
		lastMoveY = cot;

		turnTime = 15;
		lastTime = SDL_GetTicks();

		return true;
	}
	return false;
}

int checkWin(int x, int y) {
	int currentVal = GameBoard[x][y];
	// Kiểm tra Dọc
	int cnt = 0;
	for (int i = -4; i <= 4; i++) {
		if (x + i >= 0 && x + i < BOARD_SIZE) {
			if (GameBoard[x + i][y] == currentVal) {
				cnt++;
				if (cnt == 5) return currentVal;
			}
			else cnt = 0;
		}
	}
	// Kiểm tra Ngang
	cnt = 0;
	for (int i = -4; i <= 4; i++) {
		if (y + i >= 0 && y + i < BOARD_SIZE) {
			if (GameBoard[x][y + i] == currentVal) {
				cnt++;
				if (cnt == 5) return currentVal;
			}
			else cnt = 0;
		}
	}
	// Kiểm tra Chéo Chính
	cnt = 0;
	for (int i = -4; i <= 4; i++) {
		if (x + i >= 0 && x + i < BOARD_SIZE && y + i >= 0 && y + i < BOARD_SIZE) {
			if (GameBoard[x + i][y + i] == currentVal) {
				cnt++;
				if (cnt == 5) return currentVal;
			}
			else cnt = 0;
		}
	}
	// Kiểm tra Chéo Phụ
	cnt = 0;
	for (int i = -4; i <= 4; i++) {
		if (x + i >= 0 && x + i < BOARD_SIZE && y - i >= 0 && y - i < BOARD_SIZE) {
			if (GameBoard[x + i][y - i] == currentVal) {
				cnt++;
				if (cnt == 5) return currentVal;
			}
			else cnt = 0;
		}
	}
	return 0;
}

bool checkDraw() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (GameBoard[i][j] == 0)
				return false;
		}
	}
	return true;
}

void SaveGame(string filename) {
	string fullPath = filename + ".txt";
	ofstream file(fullPath);
	if (file.is_open()) {
		file << playGameSelection << endl;
		file << currentPlayer << endl;
		file << countWinP1 << " " << countWinP2 << endl;
		file << countMoveP1 << " " << countMoveP2 << endl;

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				file << GameBoard[i][j] << " ";
			}
			file << endl;
		}
		file.close();
	}
	cout << "Da luu file: " << fullPath << endl;
}

bool LoadGame(string filename) {
	string fullPath = filename + ".txt";
	ifstream file(fullPath);
	if (file.is_open()) {
		int saveMode = -1;
		file >> saveMode;
		if (saveMode == 0) currentState = STATE_PvP;
		else if (saveMode == 1) currentState = STATE_PvE_easy;
		else if (saveMode == 2) currentState = STATE_PvE_medium;
		else if (saveMode == 3) currentState = STATE_PvE_hard;
		file >> currentPlayer;
		file >> countWinP1 >> countWinP2;
		file >> countMoveP1 >> countMoveP2;
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				file >> GameBoard[i][j];
			}
		}
		file.close();

		cursorRow = BOARD_SIZE / 2;
		cursorCol = BOARD_SIZE / 2;
		lastMoveX = -1;
		lastMoveY = -1;
		turnTime = 15;
		lastTime = SDL_GetTicks();
		return true;
	}
	return false;
}

void updateTimer() {
	if (currentState == STATE_PvP && !isPause) {
		Uint32 currentTime = SDL_GetTicks();

		if (currentTime - lastTime >= 1000 && winner==0) {
			turnTime--;
			lastTime = currentTime;
			
			if (turnTime <= 0) {
				if (currentPlayer == 1)
					winner = 2;
				else
					winner = 1;
				turnTime = 15;
			}
		}

	}
}

void scanSaveFiles() {
	saveFiles.clear();
	loadSelection = 0;

	for (const auto& entry : filesystem::directory_iterator(".")) {
		if (entry.path().extension() == ".txt") {
			saveFiles.push_back(entry.path().stem().string());
		}
	}
}

void deletePFile(string filename) {
	string fullPath = filename + ".txt";

	try {
		if (filesystem::exists(fullPath)) {
			filesystem::remove(fullPath);
			cout << "Da xoa file: " << fullPath << endl;
		}
	}
	catch (const filesystem::filesystem_error& err) {
		cout << "Loi xoa file: " << err.what() << endl;
	}
}

