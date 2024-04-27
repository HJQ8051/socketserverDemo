#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

struct SockInfo
{
    struct sockaddr_in addr;
    int fd;
};

struct SockInfo infos[512];

void* working(void* arg);

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8989);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));

    ret = listen(fd, 128);
    
    int max = sizeof(infos) / sizeof(struct SockInfo);
    for (int i = 0; i < max; i++)
    {
        memset(&infos[i], 0, sizeof(infos[i]));
        infos[i].fd = -1;
    }
    int addrlen = sizeof(struct sockaddr_in);
    while (1)
    {
        struct SockInfo* pinfo;
        for (int i = 0; i < max; i++)
        {
            if (infos[i].fd == -1)
            {
                pinfo = &infos[i];
                break;
            }
        }
        int cfd = accept(fd, (struct sockaddr*)&pinfo->addr, (socklen_t*)&addrlen);
        pinfo->fd = cfd;
        pthread_t tid;
        pthread_create(&tid, NULL, working, pinfo);
        pthread_detach(tid);
    }
    close(fd);
    return 0;
}

void* working(void* arg)
{
    struct SockInfo* pinfo = (struct SockInfo*)arg;
    char ip[32];
    printf("客户端的ip：%s，端口：%d\n",
        inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
        ntohs(pinfo->addr.sin_port));
    while (1)
    {
        char buff[1024];
        int len = recv(pinfo->fd, buff, sizeof(buff), 0);
        if (len > 0)
        {
            printf("client say %s\n", buff);
            send(pinfo->fd, buff, len, 0);
        }
        else if (len == 0)
        {
            printf("断开连接\n");
            break;
        }
        else
        {
            break;
        }
    }
    close(pinfo->fd);
    pinfo->fd = -1;

    return NULL;
}