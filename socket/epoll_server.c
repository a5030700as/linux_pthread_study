/*
 * @Author: your name
 * @Date: 2020-09-15 14:58:41
 * @LastEditTime: 2020-09-15 16:01:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tzc/test_code/muil_IO_socket/epoll_server.c
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9527

#define POLL_FD_MAX 100
#define LISTEN_FD 0

int main(void)
{
    printf(" start the file %s \r\n", __FILE__);

    int listen_fd,connect_fd,epoll_df ;
    socklen_t sock_len, clean_len;
    struct epoll_event  tempt,epoll_table[POLL_FD_MAX];

    struct sockaddr_in server_addr, client_addr, clean_addr;

    bzero(&server_addr, sizeof(server_addr));
    bzero(&client_addr, sizeof(client_addr));
    bzero(&clean_addr, sizeof(clean_addr));
    
    printf("step 1 : configure the server\r\n");
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(listen_fd, 50);   

    epoll_df = epoll_create(50);

    tempt.events = EPOLLIN;
    tempt.data.fd = listen_fd;
    epoll_ctl(epoll_df, EPOLL_CTL_ADD, listen_fd, &tempt);

    int  num_read = 0;
    int index = 0;
    int i = 0;
    char str_buff[100];
    char rec_buff[512];

    while (1)
    {
        printf("epoll_wait \r\n");
        num_read = epoll_wait(epoll_df, epoll_table, 50, -1);
        for(i = 0; i < num_read; i++)
        {
            if(epoll_table[i].data.fd == listen_fd)
            {
                // if(epoll_table[i].events & EPOLLIN)
                //     printf("\r\n events & EPOLLIN OJBK\r\n");
                    
            sock_len = sizeof(sock_len);
            connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &sock_len);
            printf( " the client ip %s , port %d \r\n", 
                    inet_ntop(AF_INET, &client_addr, str_buff, sizeof(str_buff)), 
                    ntohs(client_addr.sin_port));
                    
            tempt.events = EPOLLIN;
            tempt.data.fd = connect_fd;
            epoll_ctl(epoll_df, EPOLL_CTL_ADD, connect_fd, &tempt);

            }
            else
            {
                     if (!read(epoll_table[i].data.fd, rec_buff, sizeof(rec_buff)))
                    {

                        int sock_fd = epoll_table[i].data.fd;
                        clean_len = sizeof(clean_len);
                        // getsockname(poll_fd[i].fd,(struct sockaddr *)&clean_addr, &clean_len);
                        getpeername(epoll_table[i].data.fd,(struct sockaddr *)&clean_addr, &clean_len);
                        printf( " the disconnet client ip %s , port %d \r\n", 
                                inet_ntop(AF_INET, &clean_addr, str_buff, sizeof(str_buff)), 
                                ntohs(clean_addr.sin_port));                        
                        close(sock_fd);
                        printf("disconnect!\r\n");
                        epoll_ctl(epoll_df, EPOLL_CTL_DEL, sock_fd, NULL);
                    }
                    else
                    {
                         printf(" get some date \r\n");
                    } 
            }
            
        }

    }

    return 0;
}
