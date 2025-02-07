#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

pthread_mutex_t mutex;
sem_t semFull, semEmpty;

int buffer[3];
int count = 0;

void *producer()
{
    while(1){
       
       
       sem_wait(&semEmpty);
       pthread_mutex_lock(&mutex);
       
       buffer[count] = rand() % 6;
       count++;
       printf("NUMBER OF COUNT %d\n",count);
       
       
       pthread_mutex_unlock(&mutex);
       sem_post(&semFull);
    }
    
}
void *consumer()
{
    
    while(1){
        
        sem_wait(&semFull);
        pthread_mutex_lock(&mutex);
        printf("GOT THIS..%d\n", buffer[count - 1]);
        count--;
        pthread_mutex_unlock(&mutex);
        sem_post(&semEmpty);
    }
        
}

int main()
{
    srand(time(NULL));
    pthread_t th[2];
    sem_init(&semFull, 0, 0);
    sem_init(&semEmpty, 0, 3);
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 3; i++)
    {
        if (i % 2 == 0)
        {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0)
            {
                printf("not created");
            }
        }
        else
        {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0)
            {
                printf("not created");
            }
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("not created");
        }
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
}