#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define OGRENCI_SAYISI 5   
#define DILIM_SAYISI 8     

pthread_mutex_t pizzaMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pizzaCond = PTHREAD_COND_INITIALIZER;

int pizza_dilim = 0;  

void *ogrenci(void *arg) {
    int id = *(int *)arg;
    free(arg);

    while (1) {
        usleep(200000);
        usleep(rand() % 700000);
        pthread_mutex_lock(&pizzaMutex);

        while (pizza_dilim == 0) {
            printf("pizza bitmiş %d gittim .\n", id);
            pthread_cond_wait(&pizzaCond, &pizzaMutex);
        }

        pizza_dilim--;
        printf("öğrenci %d pizza yedim, kalan dilim %d\n", id, pizza_dilim);

        if (pizza_dilim == 0) {
            pthread_cond_signal(&pizzaCond);
        }

        pthread_mutex_unlock(&pizzaMutex);
        sleep(1);
    }

    return NULL;
}

void *pizza_getiren(void *arg) {
    while (1) {
        usleep(200000);
        pthread_mutex_lock(&pizzaMutex);

        if (pizza_dilim == 0) {
            printf("Pizza geliyor...\n");
            pizza_dilim = DILIM_SAYISI;  
            printf("Pizza geldi! Dilimler: %d\n", pizza_dilim);
            pthread_cond_broadcast(&pizzaCond);
        }

        pthread_mutex_unlock(&pizzaMutex);
        sleep(2);
    }

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t ogrenciler[OGRENCI_SAYISI];
    pthread_t pizzaci;


    for (int i = 0; i < OGRENCI_SAYISI; i++) {
        int *a = malloc(sizeof(int));
        *a = i+1;
        pthread_create(&ogrenciler[i], NULL, ogrenci, a);
    }

    pthread_create(&pizzaci, NULL, pizza_getiren, NULL);

    for (int i = 0; i < OGRENCI_SAYISI; i++) {
        pthread_join(ogrenciler[i], NULL);
    }
    pthread_join(pizzaci, NULL);

    return 0;
}
