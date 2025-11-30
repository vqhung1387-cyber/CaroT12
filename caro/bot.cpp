#include "header.h"
#include <cstdlib> // Cho rand()
#include <ctime>   // Cho time()
#include <algorithm> // Cho max()

// --- BOT DỄ (EASY) ---
// Logic: Tìm chặn 3 con X liên tiếp, nếu không có thì đánh random
bool canBlock(int& x, int& y) {
    int dx[4] = { 1, 0, 1, 1 };
    int dy[4] = { 0, 1, 1, -1 };

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GameBoard[r][c] != 1) continue; // Chỉ xét quân của Người (1)

            for (int k = 0; k < 4; k++) {
                int cnt = 1;
                int nx = c + dx[k];
                int ny = r + dy[k];

                // Đếm số quân liên tiếp
                while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && GameBoard[ny][nx] == 1) {
                    cnt++;
                    nx += dx[k];
                    ny += dy[k];
                }

                if (cnt >= 3) {
                    // Chặn đầu này
                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && GameBoard[ny][nx] == 0) {
                        x = ny; y = nx; return true; // Lưu ý: GameBoard[row][col] nên x là row, y là col
                    }
                    // Chặn đầu kia
                    int nxx = c - dx[k];
                    int nyy = r - dy[k];
                    if (nxx >= 0 && nxx < BOARD_SIZE && nyy >= 0 && nyy < BOARD_SIZE && GameBoard[nyy][nxx] == 0) {
                        x = nyy; y = nxx; return true;
                    }
                }
            }
        }
    }
    return false;
}

void PVE_Easy_Move() {
    int targetRow, targetCol;

    // 1. Ưu tiên chặn nếu người chơi sắp thắng
    if (canBlock(targetRow, targetCol)) {
        placeMove(targetRow, targetCol);
    }
    else {
        // 2. Đánh ngẫu nhiên vào ô trống
        do {
            targetRow = rand() % BOARD_SIZE;
            targetCol = rand() % BOARD_SIZE;
        } while (GameBoard[targetRow][targetCol] != 0);

        placeMove(targetRow, targetCol);
    }
}

// --- BOT KHÓ (HARD) ---
// Logic: Tính điểm tấn công và phòng thủ cho từng ô trống
long evaluateMove(int row, int col) {
    int dx[4] = { 1, 0, 1, 1 };
    int dy[4] = { 0, 1, 1, -1 };
    long diemTanCong[6] = { 0, 10, 100, 1000, 10000, 1000000 };
    long diemPhongThu[6] = { 0, 15, 120, 1200, 12000, 1200000 };

    long totalScore = 0;

    for (int i = 0; i < 4; i++) {
        int demBot = 0, demNguoi = 0;

        // Duyệt hướng xuôi
        int k = 1;
        while (col + k * dx[i] >= 0 && col + k * dx[i] < BOARD_SIZE &&
            row + k * dy[i] >= 0 && row + k * dy[i] < BOARD_SIZE &&
            GameBoard[row + k * dy[i]][col + k * dx[i]] == 2) {
            demBot++; k++;
        }

        // Duyệt hướng ngược
        k = 1;
        while (col - k * dx[i] >= 0 && col - k * dx[i] < BOARD_SIZE &&
            row - k * dy[i] >= 0 && row - k * dy[i] < BOARD_SIZE &&
            GameBoard[row - k * dy[i]][col - k * dx[i]] == 2) {
            demBot++; k++;
        }

        // Tương tự cho người chơi (để chặn)
        k = 1;
        while (col + k * dx[i] >= 0 && col + k * dx[i] < BOARD_SIZE &&
            row + k * dy[i] >= 0 && row + k * dy[i] < BOARD_SIZE &&
            GameBoard[row + k * dy[i]][col + k * dx[i]] == 1) {
            demNguoi++; k++;
        }
        k = 1;
        while (col - k * dx[i] >= 0 && col - k * dx[i] < BOARD_SIZE &&
            row - k * dy[i] >= 0 && row - k * dy[i] < BOARD_SIZE &&
            GameBoard[row - k * dy[i]][col - k * dx[i]] == 1) {
            demNguoi++; k++;
        }

        // Cộng điểm (giới hạn mảng tối đa 5 phần tử)
        if (demBot > 5) demBot = 5;
        if (demNguoi > 5) demNguoi = 5;

        totalScore += diemTanCong[demBot];
        totalScore += diemPhongThu[demNguoi];
    }

    // Ưu tiên đánh gần tâm bàn cờ
    int mid = BOARD_SIZE / 2;
    totalScore += max(0, BOARD_SIZE - (abs(row - mid) + abs(col - mid)));

    return totalScore;
}

void PVE_Hard_Move() {
    int bestRow = -1, bestCol = -1;
    long maxScore = -1;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GameBoard[r][c] == 0) { // Nếu ô trống
                long score = evaluateMove(r, c);
                if (score > maxScore) {
                    maxScore = score;
                    bestRow = r;
                    bestCol = c;
                }
            }
        }
    }

    // Nếu tìm được nước đi tốt nhất thì đánh
    if (bestRow != -1 && bestCol != -1) {
        placeMove(bestRow, bestCol);
    }
    else {
        // Nếu bàn cờ trống trơn (lượt đầu), đánh vào giữa
        placeMove(BOARD_SIZE / 2, BOARD_SIZE / 2);
    }
}