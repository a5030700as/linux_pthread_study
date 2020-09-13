#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char buff[512]; 
pthread_mutex_t my_mutex;
pthread_cond_t my_cond;

void * first_pthread_handle(void * arg)
{
    int i = 50;
    pthread_mutex_lock(&my_mutex);
    pthread_cond_wait(&my_cond, &my_mutex);
    while (i--)
    {
        strncpy(buff, "first_pthread_handle!!", strlen("first_pthread_handle!!"));
        printf("%s %d \r\n", buff, i);
    }

     pthread_mutex_unlock(&my_mutex);

    pthread_exit(NULL);
}

void * second_pthread_handle(void * arg)
{
    int i = 50;

    // pthread_mutex_lock(&my_mutex);
    while (i--)
    {
        strncpy(buff, "second_pthread_handle!!", strlen("second_pthread_handle!!"));
        printf("%s %d \r\n", buff, i);
    }
    // pthread_mutex_unlock(&my_mutex);

    pthread_exit(NULL);
}

void *(*p_fun[2])(void *) = {
    first_pthread_handle,
    second_pthread_handle
};



int main(void)
{
    printf(" now test without mutex");
    strncpy(buff, "this is a mutex test!", strlen("this is a mutex test!"));
    
    pthread_mutex_init(&my_mutex, NULL);
    pthread_cond_init(&my_cond, NULL);

    int id_length = 2;
    pthread_t pthread_id[id_length];
    int id = 0;
    for(id = 0; id < id_length; id++)
    {
        pthread_create(&pthread_id[id], NULL, p_fun[id], NULL);
    }

     for(id = 0; id < id_length; id++)
    {
        pthread_join(pthread_id[id], NULL);
    }   
    pthread_exit(NULL);
}