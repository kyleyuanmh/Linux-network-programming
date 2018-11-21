#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int CLIENT_PORT=8899;
char* CLIENT_IP="192.168.1.105";

int main()
{
    /*1 创建socket*/
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd == -1)
    {
        perror("udp_socket创建失败！退出客户端系统");
        exit(-1);
    }

    /*2 准备地址：需要设计一个地址结构体用于存储sock地址结构 addr 的数据*/
   struct sockaddr_in addr;
   //设置family地址格式类型
   addr.sin_family = AF_INET;

   /*绑定地址和端口；
    * htons是一个转换函数,因为操作系统中的字节排序和网络中字节排序中的字节流顺序是不统一的，所以在进行由系统转化为网络传送。
    *  htons->host to net short 由主机转换为网络顺序。
    */
   addr.sin_port = htons(CLIENT_PORT);
   addr.sin_addr.s_addr = inet_addr(CLIENT_IP);
   if (addr.sin_addr.s_addr == INADDR_NONE)
       {
           printf("Incorrect ip address!");
           close(socket_fd);
           exit(1);
       }

   /*3 进行通信*/
   char buff[512];
   socklen_t len = sizeof(addr);
   while (1)
       {
           scanf("%s",buff);
           int n;
           //addr是sockaddr_in类型需要进行强制类型转换
           n = sendto(socket_fd, buff, strlen(buff), 0, (struct sockaddr *)&addr, sizeof(addr));
           if (n < 0)//发送了n个字节
           {
               perror("sendto");
               close(socket_fd);
               break;
           }
           memset(buff,0,sizeof (buff));
           n = recvfrom(socket_fd, buff, 512, 0, (struct sockaddr *)&addr, &len);
           if (n>0)
           {
               buff[n] = 0;
               printf("received:");
               puts(buff);
           }
           else if (n==0)
           {
               printf("server closed\n");
               close(socket_fd);
               break;
           }
           else if (n == -1)
           {
               perror("recvfrom");
               close(socket_fd);
               break;
           }
       }
    return 0;
}
