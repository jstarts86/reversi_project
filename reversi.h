#ifndef REVERSI_H
#define REVERSI_H

#define IS_SERVER 0
#define IS_CLIENT 1

int flip_pieces(int board[8][8], int y, int x, int player);
int is_valid_move(int board[8][8], int y, int x, int player);
int can_make_move(int board[8][8], int player);
void display_board(int board[8][8], int currentPlayer);
void count_pieces(int board[8][8], int* p1_pieces, int* p2_pieces);
int listen_at_port(int portnum);
void chat(int conn_fd);
int connect_ipaddr_port(const char* ip, int port);
void run_reversi(int socket, int role);

#endif /* REVERSI_H */
