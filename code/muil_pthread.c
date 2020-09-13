#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

void *start_routine(void *arg)
{
    int i = (int)arg;
    sleep(i);
    printf("i am pthread: %d,  id is %lu\r\n", i, pthread_self());
    return NULL;
}
int main(int argc, char *argv[])
{
    printf("hello my pthread\r\n");

    if (argc != 2)
    {
        printf("the argc error!! should be 2 but now is %d", argc);
        return -1;
    }

    int num = atoi(argv[1]);

    pthread_t thread_id[num];
    int i;
    for (i = 0; i < num; i++)
    {
        pthread_create(&thread_id[i], NULL, start_routine, (void *)i);
    }

    // sleep(i);
    printf("ok\r\n i am master pthread!!!\r\n");
    pthread_exit(NULL);
    //return 0;
}