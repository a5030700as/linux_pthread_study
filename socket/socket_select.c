/*
 * @Author: your name
 * @Date: 2020-09-15 09:18:30
 * @LastEditTime: 2020-09-15 11:08:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /tzc/test_code/muil_IO_socket/socket_select.c
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9527

//暂时不考虑 出错处理
/*
 注意 需要维护自己的 客户端连接描述符
 
 注意 设置的监听事件 是 传入传出参数
 
*/
int main(void)
{
    
    printf(" this is for me test select \r\n");

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_length;
    int listen_fd, connect_fd;
    char str[256];
    char rec_buff[1024];

    int client[1024];
    for(int i = 0; i < 1024; i++)
        client[i] = -1;

    /*set the server addr*/
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        perror( " bind failed : ");
    listen(listen_fd, 20);

    printf(" now server waitting for client connect.....\r\n");
    
    int max_fd = listen_fd;
    int ret_even_num = 0;

    fd_set r_set, all_set;
    FD_ZERO(&all_set);
    FD_SET(max_fd, &all_set);
 
    int i;
    int maxi = 0;

    while (1)
    {
        r_set = all_set;
        printf("  listen.....\r\n");
        ret_even_num = select(max_fd + 1, &r_set, NULL, NULL, NULL); //出错处理
        
        if(FD_ISSET(listen_fd, &r_set) != 0)  //需要增加 链接
        {
            addr_length = sizeof(addr_length);
            connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_length);
            printf("the client ip : %s, port is %d\r\n",
                    inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
                    ntohs(client_addr.sin_port));
            
            for(i = 0; i < 1024; i++)
            {
                if(client[i] < 0)
                {
                    client[i] = connect_fd;
                    break;
                }
                    
            }

            if(i == 1024)
                printf(" waring!!!! client is max!!!\r\n");

            FD_SET(connect_fd, &all_set);
            if (connect_fd > max_fd)
                max_fd = connect_fd;
            
            if (i > maxi)
                maxi = i;

            if(--ret_even_num == 0)
                continue;
        }
        for(i = 0; i < maxi + 1; i++)
        {
            if (( connect_fd = client[i]) == -1)
                continue;

            if (FD_ISSET(connect_fd, &r_set))
            {
                int length;
                if ((length = read(connect_fd, rec_buff, sizeof(rec_buff))) == 0)
                {
                    close(connect_fd);
                    printf("disconnect client\r\n");
                    FD_CLR(connect_fd, &all_set);
                    client[i] = -1;
                }
                else
                {
                    printf("get some data \r\n");
                }
                if (--ret_even_num == 0)
                    break;
            }
        }               
    }

    return 0;
}
