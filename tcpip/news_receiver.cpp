#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char* message);

int main(int argc, char* argv[]) {
    int recv_sock;
    int str_len;
    char buf[BUF_SIZE];
}