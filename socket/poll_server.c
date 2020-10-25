/*
 * @Author: your name
 * @Date: 2020-09-15 13:22:05
 * @LastEditTime: 2020-09-15 14:45:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tzc/test_code/muil_IO_socket/poll_server.c
 */
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>  
#include <arpa/inet.h>

#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9527

#define POLL_FD_MAX 100
#define LISTEN_FD 0

int main(void)
{
    
    printf(" now start the file :%s \r\n " , __FILE__);
    
    int listen_fd,connect_fd;
    socklen_t sock_len, clean_len;
    struct pollfd poll_fd[POLL_FD_MAX];
    for(int i = 0; i < POLL_FD_MAX; i++)
        poll_fd[i].fd = -1;

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
    
    printf("step 2 : configure the listen even for the listen_fd \r\n");
    poll_fd[LISTEN_FD].fd = listen_fd;
    poll_fd[LISTEN_FD].events = POLLIN;

    int ret_num = 0;
    int max_idex = 0;
    char str_buff[100];
    char rec_buff[256];
    int i = 0;

    while (1)
    {
        
        printf(" waiting for event.... \r\n");
        ret_num = poll(poll_fd, max_idex + 1, -1);

        if (poll_fd[LISTEN_FD].revents & POLLIN)
        {
            sock_len = sizeof(sock_len);
            connect_fd = accept(poll_fd[LISTEN_FD].fd, (struct sockaddr *)&client_addr, &sock_len);
            printf( " the client ip %s , port %d \r\n", 
                    inet_ntop(AF_INET, &client_addr, str_buff, sizeof(str_buff)), 
                    ntohs(client_addr.sin_port));

            for(i = 1; i < POLL_FD_MAX; i++)
            {
                if (poll_fd[i].fd == -1)
                {
                    poll_fd[i].fd = connect_fd;
                    poll_fd[i].events = POLLIN;
                    break;
                }
            }

            if (i == POLL_FD_MAX)
                printf(" waring!!!! poll is max!!!\r\n");
            
            if (i > max_idex)
                max_idex = i;
            
            if (--ret_num == 0)
                continue;
        }
        else
        {
            for(i = 1; i <= max_idex; i++)
            {
                if(poll_fd[i].revents & POLLIN)
                {
                    if (!read(poll_fd[i].fd, rec_buff, sizeof(rec_buff)))
                    {

                        clean_len = sizeof(clean_len);
                        // getsockname(poll_fd[i].fd,(struct sockaddr *)&clean_addr, &clean_len);
                        getpeername(poll_fd[i].fd,(struct sockaddr *)&clean_addr, &clean_len);
                        printf( " the disconnet client ip %s , port %d \r\n", 
                                inet_ntop(AF_INET, &clean_addr, str_buff, sizeof(str_buff)), 
                                ntohs(clean_addr.sin_port));                        
                        close(poll_fd[i].fd);
                        printf("disconnect!\r\n");
                        poll_fd[i].fd = -1;   
                    }
                    else
                    {
                         printf(" get some date \r\n");
                    }
                    if (--ret_num == 0)  
                        break; 
                }
            }
        }
        
    }
    close(poll_fd[0].fd);
    return 0;
}
