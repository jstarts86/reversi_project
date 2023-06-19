#include <ncurses.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <linux/socket.h>
#include "reversi.h"


int flip_pieces(int board[8][8], int y, int x, int player) {
    int flipped = 0;
    int opponent = 3 - player;
    int directions[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

    for (int i = 0; i < 8; i++) {
        int dy = directions[i][0], dx = directions[i][1];
        int ny = y + dy, nx = x + dx;

        int flippables[8][2]; // store potential flips
        int flip_count = 0;

        while (ny >= 0 && ny < 8 && nx >= 0 && nx < 8 && board[ny][nx] == opponent) {
            flippables[flip_count][0] = ny;
            flippables[flip_count][1] = nx;
            flip_count++;
            ny += dy;
            nx += dx;
        }
        if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8 && board[ny][nx] == player) {
            board[y][x] = player; // Place the new piece
            for (int j = 0; j < flip_count; j++) {
                board[flippables[j][0]][flippables[j][1]] = player;
            }
            flipped += flip_count;
        }
    }

    return flipped;
}


int is_valid_move(int board[8][8], int y, int x, int player) {
    if (board[y][x] != 0) {
        return 0;
    }

    int opponent = 3 - player;
    int directions[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
    for (int i = 0; i < 8; i++) {
        int dy = directions[i][0], dx = directions[i][1];
        int ny = y + dy, nx = x + dx;
        if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8 && board[ny][nx] == opponent) {
            while (ny + dy >= 0 && ny + dy < 8 && nx + dx >= 0 && nx + dx < 8 && board[ny + dy][nx + dx] == opponent) {
                ny += dy;
                nx += dx;
            }
            if (ny + dy >= 0 && ny + dy < 8 && nx + dx >= 0 && nx + dx < 8 && board[ny + dy][nx + dx] == player) {
                return 1;
            }
        }
    }

    return 0;
}

int can_make_move(int board[8][8], int player) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (is_valid_move(board, y, x, player)) {
                return 1;
            }
        }
    }
    return 0;
}

void display_board(int board[8][8], int currentPlayer) {
    wclear(stdscr);

    printw("  0 1 2 3 4 5 6 7\n");
    for (int y = 0; y < 8; y++) {
        printw("%d ", y);
        for (int x = 0; x < 8; x++) {
            if (board[y][x] == 1) {
                printw("X ");
            } else if (board[y][x] == 2) {
                printw("O ");
            } else if (is_valid_move(board, y, x, currentPlayer)) {
                printw("? ");
            } else {
                printw(". ");
            }
        }
        printw("\n");
    }

    wrefresh(stdscr);
}
