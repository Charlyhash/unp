#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SA struct sockaddr
#define MAXLINE 1024

void err_exit(const char* m)
{
    perror(m);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        printf("usage : a.out <IP address>\n");
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_exit("socket error!");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(7777);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
        err_exit("inet_pton error");

    if (connect(sockfd, (SA*)(&servaddr), sizeof(servaddr)) < 0)
        err_exit("connect error!");

    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF)
            err_exit("fputs error.");
    }
    if (n < 0)
        err_exit("read error.");

    exit(0);
}
