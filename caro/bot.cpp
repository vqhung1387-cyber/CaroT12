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
const int VH_INF = 1000000000;
const int VH_WIN       = 100000000;
const int VH_OPEN_4    = 10000000;
const int VH_CLOSED_4  = 100000;
const int VH_OPEN_3    = 50000;
const int VH_CLOSED_3  = 1000;
const int VH_OPEN_2    = 500;

bool vh_hasNeighbor(int r, int c) {
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (i == 0 && j == 0) continue;
            int nr = r + i, nc = c + j;
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && GameBoard[nr][nc] != 0) 
                return true;
        }
    }
    return false;
}

bool vh_checkWinLocal(int r, int c, int player) {
    int dx[4] = { 1, 0, 1, 1 };
    int dy[4] = { 0, 1, 1, -1 };
    for (int i = 0; i < 4; i++) {
        int count = 1;
        int nr = r + dy[i], nc = c + dx[i];
        while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && GameBoard[nr][nc] == player) {
            count++; nr += dy[i]; nc += dx[i];
        }
        nr = r - dy[i]; nc = c - dx[i];
        while (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && GameBoard[nr][nc] == player) {
            count++; nr -= dy[i]; nc -= dx[i];
        }
        if (count >= 5) return true;
    }
    return false;
}

int vh_getScore(int count, int blocks, bool isBot) {
    if (blocks == 2) return 0;
    if (count >= 5) return VH_WIN;
    if (count == 4) return (blocks == 0) ? VH_OPEN_4 : VH_CLOSED_4;
    if (count == 3) return (blocks == 0) ? VH_OPEN_3 : VH_CLOSED_3;
    if (count == 2) return (blocks == 0) ? VH_OPEN_2 : 10;
    return 1;
}

int vh_evaluateBoard() {
    int totalScore = 0;
    int dx[4] = { 1, 0, 1, 1 };
    int dy[4] = { 0, 1, 1, -1 };

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GameBoard[r][c] == 0) continue;

            int player = GameBoard[r][c];
            bool isBot = (player == 2); 

            for (int dir = 0; dir < 4; dir++) {
                int prevR = r - dy[dir], prevC = c - dx[dir];
                if (prevR >= 0 && prevR < BOARD_SIZE && prevC >= 0 && prevC < BOARD_SIZE && GameBoard[prevR][prevC] == player) continue;

                int count = 0, currR = r, currC = c;
                while (currR >= 0 && currR < BOARD_SIZE && currC >= 0 && currC < BOARD_SIZE && GameBoard[currR][currC] == player) {
                    count++; currR += dy[dir]; currC += dx[dir];
                }

                int blocks = 0;
                if (prevR < 0 || prevR >= BOARD_SIZE || prevC < 0 || prevC >= BOARD_SIZE || GameBoard[prevR][prevC] != 0) blocks++;
                if (currR < 0 || currR >= BOARD_SIZE || currC < 0 || currC >= BOARD_SIZE || GameBoard[currR][currC] != 0) blocks++;

                int score = vh_getScore(count, blocks, isBot);
                
                if (isBot) totalScore += score;
                else totalScore -= (score * 2);
            }
        }
    }
    return totalScore;
}

int vh_minimax(int depth, bool isBot, int alpha, int beta) {
    if (depth == 0) return vh_evaluateBoard();

    bool hasMove = false;
    int bestVal = isBot ? -VH_INF : VH_INF;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GameBoard[r][c] == 0 && vh_hasNeighbor(r, c)) {
                hasMove = true;
                
                GameBoard[r][c] = isBot ? 2 : 1;
                
                if (vh_checkWinLocal(r, c, isBot ? 2 : 1)) {
                    GameBoard[r][c] = 0;
                    return isBot ? VH_WIN : -VH_WIN;
                }

                int val = vh_minimax(depth - 1, !isBot, alpha, beta);
                
                GameBoard[r][c] = 0;

                if (isBot) {
                    if (val > bestVal) bestVal = val;
                    if (bestVal > alpha) alpha = bestVal;
                } else {
                    if (val < bestVal) bestVal = val;
                    if (bestVal < beta) beta = bestVal;
                }

                if (beta <= alpha) return bestVal;
            }
        }
    }
    
    if (!hasMove) return 0;
    return bestVal;
}

void PVE_VeryHard_Move() {
    int blockR = -1, blockC = -1;
    
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GameBoard[r][c] == 0 && vh_hasNeighbor(r,c)) {
                GameBoard[r][c] = 2; 
                if (vh_checkWinLocal(r, c, 2)) {
                    placeMove(r, c); return;
                }
                GameBoard[r][c] = 0;

                GameBoard[r][c] = 1;
                if (vh_checkWinLocal(r, c, 1)) {
                    blockR = r; blockC = c;
                }
                GameBoard[r][c] = 0;
            }
        }
    }

    if (blockR != -1) {
        placeMove(blockR, blockC);
        return;
    }

    int bestVal = -VH_INF;
    int moveR = -1, moveC = -1;
    
    int mid = BOARD_SIZE / 2;
    if (GameBoard[mid][mid] == 0 && !vh_hasNeighbor(mid, mid)) {
        placeMove(mid, mid); return;
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (GameBoard[r][c] == 0 && vh_hasNeighbor(r, c)) {
                GameBoard[r][c] = 2;
                
                int val = vh_minimax(4, false, -VH_INF, VH_INF); 
                
                val += (BOARD_SIZE - (abs(r - mid) + abs(c - mid)));

                GameBoard[r][c] = 0;

                if (val > bestVal) {
                    bestVal = val;
                    moveR = r; moveC = c;
                }
            }
        }
    }
    if (moveR != -1) placeMove(moveR, moveC);
    else PVE_Easy_Move();
}
