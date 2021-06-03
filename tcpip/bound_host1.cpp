#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in my_addr, you_addr;
    char message[BUF_SIZE];
    int str_len, i;
    socklen_t adr_sz;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        error_handling("bind() error");

    for (i = 0; i < 3; i++)
    {
        sleep(5);
        adr_sz = sizeof(you_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&you_addr, &adr_sz);

        printf("Message %d from server: %s\n", i + 1, message);
    }
    // if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    //     error_handling("connect() error");
    // else
    //     printf("Connected........");
    // while (1)
    // {
    //     fputs("Input message(Q to quit):", stdout);
    //     fgets(message, BUF_SIZE, stdin);

    //     if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
    //         break;
    //     sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //     // str_len = write(sock, message, strlen(message));
    //     adr_sz = sizeof(from_addr);
    //     str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&from_addr, &adr_sz);
    //     message[str_len] = 0;
    //     printf("Message from server: %s", message);
    // }
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}