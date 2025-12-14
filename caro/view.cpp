#include "header.h"
#include "img.h"

bool mainMusic = true;
bool cSound = true;
int scrollX = SCREEN_WIDTH;


void renderText(string text, int x, int y, SDL_Color color) {
	if (_font1 == NULL) return;

	// 1. Tạo Surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(_font1, text.c_str(), color);
	if (textSurface == NULL) return;

	// 2. Tạo Texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (textTexture == NULL) {
		SDL_FreeSurface(textSurface);
		return;
	}
	// 3. Tính toán vị trí vẽ (Căn giữa tâm x, y)
	int textW = textSurface->w;
	int textH = textSurface->h;
	SDL_Rect renderQuad = { x - textW / 2, y - textH / 2, textW, textH };

	SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}
void renderText(string text, int x, int y, SDL_Color color, TTF_Font* font) {
	if (font == NULL) return;

	// 1. Tạo Surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (textSurface == NULL) return;

	// 2. Tạo Texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (textTexture == NULL) {
		SDL_FreeSurface(textSurface);
		return;
	}
	// 3. Tính toán vị trí vẽ (Căn giữa tâm x, y)
	int textW = textSurface->w;
	int textH = textSurface->h;
	SDL_Rect renderQuad = { x - textW / 2, y - textH / 2, textW, textH };

	SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}
void renderText(string text, int x, int y, SDL_Color color, TTF_Font* font, float scale) {
	// 1. Kiểm tra an toàn: Nếu font chưa load hoặc chuỗi rỗng thì không làm gì cả
	if (font == NULL || text.empty()) return;

	// 2. Tạo Surface từ text (Dùng Blended cho chữ mượt, đẹp hơn Solid)
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (textSurface == NULL) return;

	// 3. Tạo Texture từ Surface
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (textTexture == NULL) {
		SDL_FreeSurface(textSurface); // Nhớ giải phóng nếu lỗi
		return;
	}

	// 4. Tính toán kích thước sau khi phóng to/thu nhỏ (Scale)
	int finalW = (int)(textSurface->w * scale);
	int finalH = (int)(textSurface->h * scale);

	// 5. Tạo khung hình chữ nhật đích (Căn giữa tâm x, y)
	SDL_Rect renderQuad = {
		x - finalW / 2,  // Dịch sang trái một nửa chiều rộng để căn giữa
		y - finalH / 2,  // Dịch lên trên một nửa chiều cao để căn giữa
		finalW,
		finalH
	};

	// 6. Vẽ lên màn hình
	SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

	// 7. Dọn dẹp bộ nhớ (Quan trọng!)
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}
void DrawBoard() {
	SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
		// Vẽ đường dọc
	for (int i = 0; i <= BOARD_SIZE; i++) {
		SDL_RenderDrawLine(renderer, LEFT + i * CELLSIZE, TOP, LEFT + i * CELLSIZE, TOP + CELLSIZE * BOARD_SIZE);
	}
		// Vẽ đường ngang
	for (int j = 0; j <= BOARD_SIZE; j++) {
		SDL_RenderDrawLine(renderer, LEFT, TOP + j * CELLSIZE, LEFT + CELLSIZE * BOARD_SIZE, TOP + j * CELLSIZE);
	}
}
void DrawMenu() {
	for (int i = 0; i < 5; i++) {
		renderRainbowText("Welcome to our game.", SCREEN_WIDTH / 2, 150);
		renderGlitchText("TIC TAC TOE", SCREEN_WIDTH / 2, 90, _font1);
	}
	SDL_Color selectedColor = { 255, 255, 255, 255 }; 
	SDL_Color normalColor = { 224, 255, 255, 255 }; 
	vector<string> menuList = { "PLAY", "LOAD", "ABOUT", "HELP","SETTINGS", "EXIT" };
	for (int i = 0; i < menuList.size(); i++) {
		if (menuSelection == i) {
			for (int j = 0; j < 5; j++)
				renderText("* " + menuList[i] + " *", SCREEN_WIDTH / 2, 240 + 80 * i, selectedColor, _font4);
		}
		else {
			for (int j = 0; j < 5; j++)
				renderText(menuList[i], SCREEN_WIDTH / 2 , 240 + 80 * i, normalColor, _font4);
		}
	}
}
void renderAbout() {
	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Color titleColor = { 255, 250, 205, 255 };
	for (int i = 0; i < 5; i++) {
		renderText("- VNUHCM - University of Science -", SCREEN_WIDTH / 2 + 10,200, titleColor, _font4);
		renderText("Class: 25C10", SCREEN_WIDTH / 2 - 153, 250, textColor, _font3);
		renderText("Mentor: Truong Toan Thinh", SCREEN_WIDTH / 2 - 43, 280, textColor, _font3);
		renderText("Created by: Group 11", SCREEN_WIDTH / 2 - 81, 310, textColor, _font3);
		renderText("25127055 - Vuong Quoc Hung", SCREEN_WIDTH / 2 - 20, 340, textColor, _font3);
		renderText("25127132 - Bui Huu Tan Tai", SCREEN_WIDTH / 2 - 31, 370, textColor, _font3);
		renderText("25127215 - Lam Yen Ngoc", SCREEN_WIDTH / 2 - 47, 410, textColor, _font3);
		renderText("25127386 - Tran Nguyen Anh Khoa", SCREEN_WIDTH / 2 + 22, 440, textColor, _font3);
		renderText("25127507 - Nguyen Quoc Thinh", SCREEN_WIDTH / 2 - 9, 470, textColor,_font3);
	}
}
void renderHelp() {
	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Color titleColor = { 255, 250, 205, 255 };
	for (int i = 0; i < 5; i++) {
		renderText(" CONTROLS ", SCREEN_WIDTH / 2 + 10, 200, titleColor, _font4);
		renderText("- Use W, A, S, D to move P1, SPACE to place X.", 506, 250, textColor, _font3);
		renderText("- Use UP, DOWN, RIGHT, LEFT to move P2, ENTER to place O.", SCREEN_WIDTH / 2 - 34, 280, textColor, _font3);
		renderText("- Press L to save game (only in PvP).", 433, 310, textColor, _font3);
		renderText("- Two players take turns marking X and O on a grid; the first", SCREEN_WIDTH / 2 - 24, 380, textColor, _font3);
		renderText("to get 5 consecutive marks in a horizontal, vertical,or diagonal", SCREEN_WIDTH / 2 - 15, 410, textColor, _font3);
		renderText(" row wins.", 180, 440, textColor, _font3);
		renderText("- Press Esc to exit.", SCREEN_WIDTH / 2, 460, textColor, _font3);
		renderText(" RULES ", SCREEN_WIDTH / 2 + 10, 350, titleColor, _font4);


	}
}

// Chinh vi tri ham setting lai
void renderSetting() {
	SDL_Color selectedColor = { 255, 250, 205, 255 };
	SDL_Color textColor = { 255, 255, 255, 255 };

	vector<string> settingList = { "Music","SFX" "Back" };
	for (int i = 0; i < 5; i++) {
		if (settingSelection == 0) {
			if (mainMusic) {
				renderText("Music: ON", SCREEN_WIDTH / 2 - 30, 310, selectedColor, _font4);
			}
			else
				renderText("Music: OFF", SCREEN_WIDTH / 2 - 35, 310, selectedColor, _font4);
		}
		else {
			if (mainMusic) {
				renderText("Music: ON", SCREEN_WIDTH / 2 - 30, 310, textColor, _font4);
			}
			else
				renderText("Music: OFF", SCREEN_WIDTH / 2 - 35, 310, textColor, _font4);
		}
		if (settingSelection == 1) {
			if (cSound)
				renderText("SFX: ON", SCREEN_WIDTH / 2 - 40, 340, selectedColor, _font4);
			else
				renderText("SFX: OFF", SCREEN_WIDTH / 2 - 45, 340, selectedColor, _font4);
		}
		else {
			if (cSound)
				renderText("SFX: ON", SCREEN_WIDTH / 2 - 40, 340, textColor, _font4);
			else
				renderText("SFX: OFF", SCREEN_WIDTH / 2 - 45, 340, textColor, _font4);
		}
		if (settingSelection == 2) {
			renderText("Back", SCREEN_WIDTH / 2 - 50, 370, selectedColor, _font4);
		}
		else {
			renderText("Back", SCREEN_WIDTH / 2 - 50, 370, textColor, _font4);
		}
		
	}
}
void renderLoad() {
	SDL_Color selectedColor = { 255, 0, 0, 255 };
	SDL_Color textColor = { 101, 67, 33, 255 };
	if (saveFiles.empty()) {
		renderText("No save files found!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, textColor, _font3);
	}
	else {
		int startY = 300;
		for (int i = 0; i < saveFiles.size(); i++) {
			SDL_Color color;
			string textToShow;

			if (i == loadSelection) {
				color = selectedColor;
				textToShow = ">> " + saveFiles[i] + " <<";
			}
			else {
				color = textColor;
				textToShow = saveFiles[i];
			}
			renderText(textToShow, SCREEN_WIDTH / 2, startY + (i * 50), color, _font3);
		}
	}

	// 4. Hướng dẫn
	renderText("UP/DOWN: Select - ENTER: Load - ESC: Back", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150, { 150, 150, 150, 255 }, _font4);
}
void renderHello() {
	SDL_Color tcolor = { 0,0,0,255 };
	for (int i = 0; i < 5; i++)
		renderFloatingText("Press any keys to continue!", SCREEN_WIDTH / 2, 555, tcolor, _font4);
	renderScrollingText("Helloooooo !!!!!!!!!!!", 50, tcolor);
}
void DrawXO() {
	SDL_Color colorX = { 60, 180, 170, 255 }; 
	SDL_Color colorO = { 230, 100, 80, 255 }; 

	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			if (GameBoard[r][c] == 0) continue;

			int centerX = LEFT + c * CELLSIZE + CELLSIZE / 2;
			int centerY = TOP + r * CELLSIZE + CELLSIZE / 2;

			if (GameBoard[r][c] == 1) {
				for(int i = 0; i < 5; i++)
					renderText("X", centerX, centerY, colorX, _font3);
			}
			else if (GameBoard[r][c] == 2) {
				for(int i = 0; i < 5; i++)
					renderText("O", centerX, centerY, colorO, _font3);
			}
		}
	}

	// 2. VẼ HỒNG (Nước đi cuối cùng)
	if (lastMoveX != -1 && lastMoveY != -1) {
		SDL_Rect lastRect = { LEFT + lastMoveY * CELLSIZE, TOP + lastMoveX * CELLSIZE, CELLSIZE, CELLSIZE };
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Xanh lá
		SDL_RenderDrawRect(renderer, &lastRect);
	}

	// 3. VẼ CON TRỎ BÀN PHÍM (Cursor)
	// Chỉ vẽ khi đang chơi game
	if (currentState == STATE_PvP || currentState == STATE_PvE_easy || currentState == STATE_PvE_hard) {
		SDL_Rect cursorRect = { LEFT + cursorCol * CELLSIZE, TOP + cursorRow * CELLSIZE, CELLSIZE, CELLSIZE };

		// Vẽ màu Xám đậm để làm con trỏ
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

		// Vẽ 2 lớp hình chữ nhật để viền dày hơn, dễ nhìn hơn
		SDL_RenderDrawRect(renderer, &cursorRect);

		SDL_Rect innerRect = { cursorRect.x + 1, cursorRect.y + 1, cursorRect.w - 2, cursorRect.h - 2 };
		SDL_RenderDrawRect(renderer, &innerRect);
	}
}
void DrawUI() {
	SDL_Color white = { 255,255,255,255 };
	SDL_Color den = { 0,0,0,255 };
	// UI P1
	string temp = to_string(countWinP1);
	for (int i = 0; i < 5; i++)
		renderText(temp, 160, 385, den, _font2);
	temp = to_string(countMoveP1);
	for (int i = 0; i < 5; i++)
		renderText(temp, 210, 458, den, _font2);

	// UI P2
	temp = to_string(countWinP2);
	for (int i = 0; i < 5; i++)
		renderText(temp, 1110, 385, den, _font2);
	temp = to_string(countMoveP2);
	for (int i = 0; i < 5; i++)
		renderText(temp, 1165, 450, den, _font2);

	// Turn
	if(currentState == STATE_PvP)
	{
		for (int i = 0; i < 5; i++) {
			if (currentPlayer == 1) {
				renderText("Turn: Player 1", SCREEN_WIDTH / 2, 30, white, _font4);
			}
			else if (currentPlayer == 2) {
				renderText("Turn: Player 2", SCREEN_WIDTH / 2, 30, white, _font4);
			}
		}
	}

	// Timer
	if(currentState == STATE_PvP)
	{
		SDL_Color timeColor = (turnTime <= 5) ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 0, 0, 0, 255 };
		for (int i = 0; i < 5; i++)
			renderText("TIME: " + to_string(turnTime), SCREEN_WIDTH / 2, 80, timeColor, _font4);
	}

}
void renderPause() {
	SDL_Color selectedColor = { 27, 4, 102, 255 }; // Xanh đậm
	SDL_Color normalColor = { 60, 100, 223, 255 };     // Xanh Nhạt
	vector<string> pauseList = { "Back", "Settings", "Help", "Menu" };
	for (int i = 0; i < 4; i++) { 
		if (pauseSelection == i) {
			for(int j = 0; j < 5; j++)
				renderText(">> " + pauseList[i] + " <<", SCREEN_WIDTH / 2, 240 + 90 * i, selectedColor, _font4);
		}	
		else {
			for(int j = 0; j < 5; j++)
				renderText(pauseList[i], SCREEN_WIDTH / 2, 240+ 90 * i, normalColor, _font4);
		}
	}
}
void renderPlay() {
	SDL_Color title = { 10, 30, 97, 255 };
	SDL_Color selectedColor = { 27, 4, 102, 255 }; // Xanh đậm
	SDL_Color normalColor = { 60, 100, 223, 255 };// Xanh Nhạt
	for(int i = 0; i < 5; i++)
		renderRainbowText("GAMEMODE", SCREEN_WIDTH / 2, 145, _font1);
	vector<string> playList = { "PvP", "PvE - Easy", "PvE - Hard" };
	for (int i = 0; i < 3; i++) {
		if (playGameSelection == i) {
			for (int j = 0; j < 5; j++)
				renderFloatingText(playList[i], SCREEN_WIDTH / 2 + 15, 320 + 150 * i, selectedColor, _font2);
		}
		else {
			for(int j = 0; j < 5; j++)
				renderText(playList[i], SCREEN_WIDTH / 2 + 15, 320 + 150 * i, normalColor, _font2);

		}
	}
}
SDL_Texture* createTextTexture(string text, SDL_Color color) {
	if (_font1 == NULL) return NULL;
	SDL_Surface* surf = TTF_RenderText_Solid(_font1, text.c_str(), color);
	if (surf == NULL) return NULL;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	return tex;
}

void DrawNameInput() {
	SDL_Color textColor = { 0, 0, 255, 255 }; 
	SDL_Color hintColor = { 100, 100, 100, 255 };
	SDL_Color deepPurple = { 86, 65, 135 };
	for (int i = 0; i < 5; i++)
		renderRainbowText("Enter file name:", SCREEN_WIDTH / 2, 330);

	// 4. Vẽ nội dung người dùng đang gõ
	// Thêm dấu gạch dưới "_" nhấp nháy để tạo cảm giác đang gõ
	string displayString = inputText;
	if ((SDL_GetTicks() / 500) % 2 == 0) displayString += "_";

	if (inputText.length() > 0) {
		renderText(inputText, SCREEN_WIDTH / 2, 380, textColor, _font4);
	}
	else {
		renderText("_", SCREEN_WIDTH / 2, 380, deepPurple, _font4);
	}

	// 5. Hướng dẫn
	for(int i = 0; i < 5; i++)
		renderText("(Enter to Confirm - Esc to Cancel)", SCREEN_WIDTH / 2, 580, hintColor, _font4);
}

void initSnow() {
	for (int i = 0; i < MAX_SNOW; i++) {
		snow[i].x = rand() % SCREEN_WIDTH; // Vị trí X ngẫu nhiên
		snow[i].y = rand() % SCREEN_HEIGHT * -1; // Bắt đầu từ phía trên màn hình
		snow[i].speed = (rand() % 3 + 1);  // Tốc độ ngẫu nhiên từ 1 đến 3
		snow[i].size = (rand() % 3 + 2);   // Kích thước ngẫu nhiên từ 2 đến 4 pixel
	}
}

void renderSnowEffect() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200); // Màu trắng hơi trong suốt

	for (int i = 0; i < MAX_SNOW; i++) {
		// 1. Cập nhật vị trí (Rơi xuống)
		snow[i].y += snow[i].speed;

		// Nếu rơi quá màn hình thì reset lại lên trên cùng
		if (snow[i].y > SCREEN_HEIGHT) {
			snow[i].y = rand() % 50 * -1; // Reset vị trí Y trên cùng
			snow[i].x = rand() % SCREEN_WIDTH; // Random lại vị trí X mới
		}

		// 2. Vẽ bông tuyết
		SDL_Rect snowRect = { (int)snow[i].x, (int)snow[i].y, snow[i].size, snow[i].size };
		SDL_RenderFillRect(renderer, &snowRect);
	}
}


