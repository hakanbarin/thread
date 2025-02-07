#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <nl_types.h>
#include <time.h>
#include <semaphore.h>

int buffer[4];
int count = 0;

pthread_barrier_t barrier;
sem_t semEmpty, semFull;
pthread_mutex_t mutex;

void *pizza_getiren(void *args)
{
    while (1)
    {
        if (count < 4)
        {
            sem_wait(&semEmpty);
            pthread_mutex_lock(&mutex);
            buffer[count] = 1;
            ++count;
            printf("%d\n", count);
            pthread_mutex_unlock(&mutex);
        }
            sem_post(&semFull);
    }
}
void *ogrenci1()
{
    while(1){

    pthread_mutex_lock(&mutex);
    usleep(100000);
    sem_wait(&semFull);
    printf("pizzayı yedim\n");
    buffer[count - 1] = 0;
    count--;
    if(buffer[0] == 0){
        while(1){
        sem_post(&semEmpty);
        }
    }
    pthread_mutex_unlock(&mutex);
}
}
void *ogrenci2()
{
}
void *ogrenci3()
{
}
void *ogrenci4()
{
}

int main()
{
    srand(time(NULL));
    pthread_t th[2];
    sem_init(&semEmpty, 0, 4);
    sem_init(&semFull, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 2; i++)
    {
        if (i % 2 == 0)
        {
            if (pthread_create(&th[i], NULL, &pizza_getiren, NULL) != 0)
            {
                perror("failed producer");
            }
        }
        else
        {

            if (pthread_create(&th[i], NULL, &ogrenci1, NULL) != 0)
            {
                perror("failed costumer ");
            }
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("failed to join");
        }
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
}