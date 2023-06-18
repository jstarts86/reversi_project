#include <ncurses.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

int flip_pieces(int board[8][8], int y, int x, int player) {
    int flipped = 0;
    int opponent = 3 - player;
    int directions[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
}
