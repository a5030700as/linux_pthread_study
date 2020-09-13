#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>


char buff[512]; 
pthread_mutex_t my_mutex;
sem_t my_sem;

void * first_pthread_handle(void * arg)
{
    int i = 50;
    // pthread_mutex_lock(&my_mutex);
    int cnt = 0;
    sleep(1);
    while (i--)
    {
        sem_post(&my_sem);      
        // sem_getvalue(&my_sem, &cnt);
        // pthread_mutex_lock(&my_mutex);
        strncpy(buff, "first_pthread_handle!!", strlen("first_pthread_handle!!"));
        printf("%s %d --------counter %d\r\n", buff, i, cnt);
        // pthread_mutex_unlock(&my_mutex);
        //  sem_wait(&my_sem);
        // sleep(1);
    }   
    // pthread_mutex_unlock(&my_mutex);

    pthread_exit(NULL);
}

void * second_pthread_handle(void * arg)
{
    int i = 50;

    // pthread_mutex_lock(&my_mutex);
    while (i--)
    {
        sem_wait(&my_sem);
        // sem_post(&my_sem);
        strncpy(buff, "second_pthread_handle!!", strlen("second_pthread_handle!!"));
        printf("%s %d \r\n", buff, i);
        // sem_post(&my_sem);
        
        // sem_post(&my_sem);
        // sleep(1);
    }
    //  pthread_mutex_lock(&my_mutex);
    //     for (int i = 0; i < 10; i++)
    //         sem_post(&my_sem);
    //  pthread_mutex_unlock(&my_mutex);
    // pthread_mutex_unlock(&my_mutex);
    pthread_exit(NULL);
}

void *(*p_fun[2])(void *) = {
    first_pthread_handle,
    second_pthread_handle
};



int main(void)
{
    printf(" now test without mutex\r\n");
    strncpy(buff, "this is a mutex test!", strlen("this is a mutex test!"));
    pthread_mutex_init(&my_mutex, NULL);
    sem_init(&my_sem, 0, 0);

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
    printf(" test end!!!!!!\r\n"); 
    pthread_exit(NULL);
}