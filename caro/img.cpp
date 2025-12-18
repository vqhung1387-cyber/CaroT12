#include "img.h"
SDL_Texture* Menu = NULL;
SDL_Texture* Settings = NULL;
SDL_Texture* Help = NULL;
SDL_Texture* About = NULL;
SDL_Texture* Load = NULL;
SDL_Texture* PlayGame = NULL;
SDL_Texture* Hello = NULL;
SDL_Texture* Save = NULL;
SDL_Texture* Pause = NULL;
SDL_Texture* PvP = NULL;
SDL_Texture* PvE = NULL;

SDL_Texture* loadTexture(const char* pFile) {
	SDL_Surface* loadedSurface = IMG_Load(pFile);
	if (loadedSurface == NULL) {
		cout << "Khong the tai texture!" << IMG_GetError() << endl;
		return NULL;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	return newTexture;
}

void DrawBackGround() {
	SDL_SetRenderDrawColor(renderer, 250, 248, 239, 255);
	SDL_RenderClear(renderer);

	SDL_Texture* currentBg = NULL;

	if (currentState == STATE_MENU) {
		currentBg = Menu;
	}
	else if (currentState == STATE_PvP) {
		currentBg = PvP;
	}
	else if (currentState == STATE_ABOUT) {
		currentBg = About;
	}
	else if (currentState == STATE_HELP) {
		currentBg = Help;
	}
	else if (currentState == STATE_SETTING) {
		currentBg = Settings;
	}
	else if (currentState == STATE_LOAD) {
		currentBg = Load;
	}
	else if (currentState == STATE_HELLO) {
		currentBg = Hello;
	}
	else if (currentState == STATE_SAVE) {
		currentBg = Save;
	}
	else if (currentState == STATE_PAUSE) {
		currentBg = Pause;
	}
	else if (currentState == STATE_PLAYING) {
		currentBg = PlayGame;
	}
	else if (currentState == STATE_PvE_easy || currentState == STATE_PvE_hard || currentState == STATE_PvE_medium) {
		currentBg = PvE;
	}
	if (currentBg != NULL) {
		SDL_RenderCopy(renderer, currentBg, NULL, NULL);
	}
}
// Effects

void renderWinEffect(int winner) {
	// 1. LÀM TỐI NỀN (Dim Background)
	// Giúp chữ nổi bật hơn trên bàn cờ

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); // Màu đen mờ (Alpha 150)
	SDL_Rect fullScreen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(renderer, &fullScreen);

	// 2. XÁC ĐỊNH MÀU SẮC VÀ NỘI DUNG
	string winMsg;
	SDL_Color mainColor;

	if (winner == 1) {
		winMsg = "PLAYER X WINS!";
		mainColor = { 255, 69, 69, 255 }; // Red
	}
	else if (winner == 2) {
		winMsg = "PLAYER O WINS!";
		mainColor = { 50, 205, 50, 255 }; // Green
	}
	else {
		winMsg = "DRAW GAME!";
		mainColor = { 200, 200, 200, 255 }; // Grey
	}

	// 3. HIỆU ỨNG "PHÁO HOA" TOÁN HỌC (Math-based Confetti)
	// Tạo ra các hạt bay xung quanh mà không cần tạo biến lưu trữ
	double time = SDL_GetTicks() / 1000.0;

	for (int i = 0; i < 30; i++) {
		// Tính toán vị trí		dựa trên thời gian và chỉ số i
		// Hạt sẽ bay theo quỹ đạo hình Lissajous hoặc Sin/Cos ngẫu nhiên
		int x = SCREEN_WIDTH / 2 + sin(time * 2 + i) * 300;
		int y = SCREEN_HEIGHT / 2 + cos(time * 3 + i * 2) * 200;

		// Màu sắc thay đổi lấp lánh
		SDL_Color pColor = mainColor;
		if (i % 2 == 0) pColor = { 255, 255, 0, 255 }; // Xen kẽ màu Vàng

		SDL_SetRenderDrawColor(renderer, pColor.r, pColor.g, pColor.b, 255);

		// Vẽ hạt hình vuông nhỏ xoay quanh tâm
		SDL_Rect pRect = { x, y, 8, 8 };
		SDL_RenderFillRect(renderer, &pRect);
	}

	// 4. VẼ CHỮ CHIẾN THẮNG (Hiệu ứng Nhịp tim / Pulsing)
	// Scale dao động từ 1.0 đến 1.2
	float scale = 1.0f + (sin(SDL_GetTicks() / 200.0) * 0.1f);

	// Vẽ bóng đổ cho chữ (Shadow)
	renderText(winMsg, SCREEN_WIDTH / 2 + 4, SCREEN_HEIGHT / 2 + 4, { 50, 50, 50, 255 }, _font2, scale);

	// Vẽ chữ chính
	renderText(winMsg, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, mainColor, _font2, scale);

	// 5. HƯỚNG DẪN NGƯỜI CHƠI
	SDL_Color hintColor = { 255, 255, 255, 255 };
	// Chữ nhấp nháy chậm
	if ((SDL_GetTicks() / 500) % 2 == 0) {
		renderText("Press Y to Replay - N to Menu", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, hintColor, _font4, 1.0f);
	}
}

void renderFloatingText(string text, int x, int y, SDL_Color color, TTF_Font* font) {

	double time = SDL_GetTicks() / 1000.0;

	int offset = 10 * sin(5.0 * time);

	renderText(text, x, y + offset, color, font);
}

void renderRainbowText(string text, int x, int y) {
	// Hiệu ứng sáng tối nhẹ nhàng (Breathing)
	double time = SDL_GetTicks() / 800.0;
	Uint8 alpha = 150 + (sin(time) + 1) * 50; // Độ trong suốt thay đổi

	// Màu VÀNG KIM (Gold)
	SDL_Color goldColor = { 255, 215, 0, alpha };

	// Vẽ chữ (Dùng font nhỏ _font2 hoặc _font3)
	renderText(text, x, y, goldColor, _font2);
}

void renderRainbowText(string text, int x, int y, TTF_Font* font) {
	double time = SDL_GetTicks() / 500.0;

	// Chuyển từ Trắng (255,255,255) sang Xanh (135,206,250)
	// Kênh R giảm từ 255 xuống thấp, G giảm ít, B giữ nguyên cao

	Uint8 val = (sin(time) + 1) * 60 + 135; // Dao động khoảng 135 -> 255

	SDL_Color frozenColor = { val, 255, 255, 255 };
	// Mẹo: Để R = val sẽ tạo ra màu Cyan nhạt lấp lánh sang trắng

	renderText(text, x, y, frozenColor, _font2);
}

void renderScrollingText(string text, int y, SDL_Color color) {
	// Mỗi khung hình trừ đi 2 pixel (Tốc độ chạy)
	scrollX -= 1;

	// Nếu chữ chạy hết màn hình bên trái -> Reset về bên phải
	// (Giả sử chữ dài khoảng 200px, nên để -200 để nó khuất hẳn mới reset)
	if (scrollX < -300) {
		scrollX = SCREEN_WIDTH;
	}

	renderText(text, scrollX, y, color, _font4);
}

void renderTextWithShadow(string text, int x, int y, SDL_Color mainColor, TTF_Font* font) {
	// 1. Vẽ bóng màu xám đậm (lệch xuống dưới bên phải 2px)
	SDL_Color shadowColor = { 50, 50, 50, 100 }; // Xám
	renderText(text, x + 2, y + 2, shadowColor, font);

	// 2. Vẽ chữ chính đè lên
	renderText(text, x, y, mainColor, font);
}

void renderPulsingText(string text, int x, int y, SDL_Color color) {
	// Lấy thời gian để tạo nhịp
	double time = SDL_GetTicks() / 500.0; // Chia 500 để nhịp chậm vừa phải

	// Công thức: Scale dao động từ 1.0 đến 1.2
	// sin(time) ra giá trị từ -1 đến 1 -> (sin + 1) từ 0 đến 2 -> chia 10 là 0 đến 0.2
	float scale = 1.0f + (sin(time) * 0.1f);

	// Gọi hàm renderText có scale
	// renderText(text, x, y, color, scale, _font3);
}

void renderGlitchText(string text, int x, int y, TTF_Font* font) {
	// Tạo hiệu ứng nảy nhẹ theo nhịp
	double time = SDL_GetTicks() / 500.0;
	int offsetY = sin(time) * 5;

	// 1. Bóng đổ màu TRẮNG TUYẾT (Lệch xuống dưới 3px)
	SDL_Color snowShadow = { 255, 255, 255, 255 };
	renderText(text, x + 3, y + offsetY + 3, snowShadow, font);

	// 2. Chữ chính màu ĐỎ NOEL (Santa Red)
	SDL_Color santaRed = { 255, 255, 255, 255 };
	renderText(text, x, y + offsetY, santaRed, font);
}

void renderShakingText(string text, int x, int y, SDL_Color color, TTF_Font* font) {
	// Cường độ rung (Intensity)
	int intensity = 4;

	// Random vị trí lệch đi một chút
	int offsetX = (rand() % intensity) - (intensity / 2);
	int offsetY = (rand() % intensity) - (intensity / 2);

	renderText(text, x + offsetX, y + offsetY, color, font);
}