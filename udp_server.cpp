#include <sys/types.h>
#include <sys/socket.h>
#include<pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include<arpa/inet.h>
//#include<cstdlib.h>

char* PORT="8899"; //server's port
char* SOCKET_SERVER_IP="192.168.1.105"; //


int main()
{
    printf("Welcome! This is a UDP server, I can only received message from client and reply with same message\n");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(PORT));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int sock;
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    //port bind to server
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    char buff[512];
    struct sockaddr_in clientAddr;
    memset(&clientAddr,0,sizeof(clientAddr));
    size_t n;
    socklen_t len = sizeof(clientAddr);
    while (1)
    {
        n = recvfrom(sock, buff, 511, 0, (struct sockaddr*)&clientAddr, &len);
        if (n>0)
        {
            buff[n] = 0;
            printf("%s %u says: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
            n = sendto(sock, buff, n, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
            if (n < 0)
            {
                perror("sendto");
                break;
            }
        }
        else
        {
            perror("recv");
            break;
        }
    }
    return 0;
}
