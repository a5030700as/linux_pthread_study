#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* my_start_routine(void * arg)
{

    printf("hello  start_routine\r\n");
    return 0;

}

int main(void)
{

    pthread_t first_pthread = pthread_self();
    printf(" hello my pthread! %lu \r\n", first_pthread);
    if (0 != pthread_create(&first_pthread, NULL, my_start_routine, NULL)) 
    {
        printf(" error !!! ");
    }
    sleep(1);
    return 0;
}
