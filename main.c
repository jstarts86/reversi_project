#include "reversi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>


int main(int argc, char* argv[]) {
    if (strcmp(argv[1], "server") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s server [port]\n", argv[0]);
            return 1;
        }
        int port = atoi(argv[2]);
        int socket = listen_at_port(port);
        run_reversi(socket, IS_SERVER);
    } else if (strcmp(argv[1], "client") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage: %s client [ip address] [port]\n", argv[0]);
            return 1;
        }
        char* ip = argv[2];
        int port = atoi(argv[3]);
        int socket = connect_ipaddr_port(ip, port);
        run_reversi(socket, IS_CLIENT);
        shutdown(socket, SHUT_RDWR);
    } else {
        fprintf(stderr, "Invalid argument: %s. Expect 'server' or 'client'\n", argv[1]);
        return 1;
    }

    return 0;
}
