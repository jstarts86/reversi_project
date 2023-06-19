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

void count_pieces(int board[8][8], int* p1_pieces, int* p2_pieces) {
    *p1_pieces = 0;
    *p2_pieces = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board[y][x] == 1) {
                (*p1_pieces)++;
            } else if (board[y][x] == 2) {
                (*p2_pieces)++;
            }
        }
    }
}

int flip_pieces(int board[8][8], int y, int x, int player) {
    int flipped = 0;
    int opponent = 3 - player;
    int directions[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

}

int listen_at_port(int portnum) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == 0) {
        perror("socket failed : ");
        exit(EXIT_FAILURE);
    }
    int opt = 2;
    if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
        fprintf(stderr, "fail at setsockopt\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portnum);

    if (bind(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed: ");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, 16) < 0) {
        perror("listen failed : ");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);
    int conn_fd = accept(sock_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (conn_fd < 0) {
        perror("accept failed: ");
        exit(EXIT_FAILURE);
    }
    return conn_fd;
}

void chat(int conn_fd) {
    char buf[256];

    do {
        int s;

        while ((s = recv(conn_fd, buf, 255, 0)) == 0)
            ;
        if (s == -1)
            break;

        buf[s] = '\0';
        printf(">%s\n", buf);

        fgets(buf, 256, stdin);
        buf[strlen(buf) - 1] = '\0';
        if (strcmp(buf, "quit()") == 0)
            break;

        send(conn_fd, buf, strlen(buf), 0);

    } while (strcmp(buf, "quit()") != 0);
}

int connect_ipaddr_port(const char* ip, int port) {
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd <= 0) {
        perror("socket failed : ");
        exit(EXIT_FAILURE);
    }
    int opt = 2;
    if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
        fprintf(stderr, "fail at setsockopt\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
        perror("inet_pton failed : ");
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("connect failed : ");
        exit(EXIT_FAILURE);
    }
    return sock_fd;
}

