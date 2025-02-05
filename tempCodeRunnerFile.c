// 16. video baya baya güzel bilgi veriyor pthread_t ile gettid arasındaki farkı anlatıyor operating system--pthread api farkı

// FONKSİYONLARDAN GERİ DÖNÜŞ DEĞERİ OLARAK ADRES ALIRSAK HATA ALIRIZ ÇÜNKÜ FONKSİYON BİTİNCE
// DEĞİŞKENLER DEALLOCATED EDİLİYOR AŞIRI ÖNEMLİ DİKKAT ET HEAPTE YER AÇIP (MALLOC İLE) ONU DÖNDÜRÜRSEK
// SORUN KALMAZ

// void* değerini bir değişkene atayamadığımız için int* yaptıktan sonra değişken dönüşümü uyguluyoruz.!!!!!!!!

// BARİYER KISMINA BAŞKA VİDEOLARDAN BAK 15. VİDEOYU TEKRAR İZLE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <nl_types.h>
#include <time.h>

#define THREAD_NUM 8

pthread_mutex_t mutexFuel;
int fuel = 50;
pthread_mutex_t mutexWater;
int water = 10;

void* routine(void* args) {
    if (rand() % 2 == 0) {
        pthread_mutex_lock(&mutexFuel);
        usleep(100000);
        pthread_mutex_lock(&mutexWater);
    }

    fuel += 50;
    water = fuel;
    printf("Incremented fuel to: %d set water to %d\n", fuel, water);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexWater);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_mutex_init(&mutexWater, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    printf("Fuel: %d\n", fuel);
    printf("Water: %d\n", water);
    pthread_mutex_destroy(&mutexFuel);
    pthread_mutex_destroy(&mutexWater);
    return 0;
}











/*
void *routine()
{

    printf("bariyeri bekliyorum\n");
    pthread_barrier_wait(&barrier);
    printf("bariyer asildi");
}

int main()
{

    pthread_t th[2];
    pthread_barrier_init(&barrier, NULL,3); //bariyer 3 thread olana kadar başlamıyor bekliyor
    for (int i = 0; i < 3; ++i)
    {

        if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
        {
            printf("non create");
        }
    }

    for (int i = 0; i < 3; ++i)
    {

        if (pthread_join(th[i], NULL) != 0)
        {
            printf("non join");
        }
    }

    pthread_barrier_destroy(&barrier);
}

*/

// pthread_exit main fonksiyonda kullanılınca main threadi kapatsa bile diğer threadleri ayakta tutuyor threadler bitene
// kadar çalışmalarına izin veriyor.
/*

void* randomsayi(){
    int * toplam = malloc(sizeof(int)); //void * ı herhangi birine ata ama voide herhangi bir şey atayamazsın
    *toplam = (rand() % 5) + 1;
    sleep(1);
    printf("toplam..%d\n",*toplam);
    pthread_exit((void *)toplam);
}


int main(){

    srand(time(NULL));
    pthread_t th;
    void *feedback;

    if(pthread_create(&th, NULL, &randomsayi, NULL) !=0){
        printf("olmadi");
    }

    pthread_exit(0);// burada return 0; yazınca tüm her şey killendiği için herhangi bir çıkış alamıyoruz fakat
                    //pthread_exit(0); sadece main threadi killiyor diğerleri ayakta kalıyor.

    pthread_join(th, &feedback);




}


*/

// trylock fazla thread az erişilmesi gereken yer olunca işe yarıyor. örnek 4 arabaya 10 sürücü binecek
// 6 kişinin sürekli beklemesi gerekiyor bunun gibi durumlar

/*
pthread_mutex_t stoveMutex[4];
int stoveFuel[4] = {100, 100, 100, 100};

int check()
{
    for (int i = 0; i < 4; i++)
    {
        if (stoveFuel[i] > 0)
        {
            return 0;
        }
    }
    return 15;
}

void *routine(void *args)
{
    while (!check())
    {
        for (int i = 0; i < 4; i++)
        {
            if (pthread_mutex_trylock(&stoveMutex[i]) == 0)
            {
                if (stoveFuel[i] > 0)
                {
                    int fuelNeeded = (rand() % 30);

                    if (stoveFuel[i] - fuelNeeded < 0)
                    {
                        printf("fuel is empty\n");
                        stoveFuel[i] = 0;
                    }
                    else
                    {
                        printf("stove...%d\n", stoveFuel[i]);
                        usleep(500000);
                        stoveFuel[i] -= fuelNeeded;
                    }
                }
                pthread_mutex_unlock(&stoveMutex[i]);
                break;
            }
            else
            {
                if (i == 3)
                {
                    usleep(300000);
                    i = 0;
                }
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    pthread_t th[10];
    for (int i = 0; i < 4; i++)
    {
        pthread_mutex_init(&stoveMutex[i], NULL);
    }
    for (int i = 0; i < 10; i++)
    {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
        {
            printf("failed to create\n");
        }
    }
    for (int i = 0; i < 10; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("failed to join\n");
        }
    }

    for (int i = 0; i < 4; i++)
    {
        pthread_mutex_destroy(&stoveMutex[i]);
    }
}
*/
/*
int fuel = 0;

void *fuelGas(){
    while (fuel != 100){
        pthread_mutex_lock(&mutexFuel);
        fuel = fuel + 10;
        printf("Gas is fuelling..%d\n",fuel);
        pthread_mutex_unlock(&mutexFuel);
        pthread_cond_broadcast(&condFuel); //tüm threadleri uyandırıyor hepsine sinyal yolluyor.
        // pthread_cond_signal(&condFuel);
        sleep(1);

    }
}

void *car(){
    pthread_mutex_lock(&mutexFuel);

    while(fuel < 60){
        printf("No fuel. Waiting...\n");
        pthread_cond_wait(&condFuel,&mutexFuel);
    }
    fuel -= 40;
    printf("Got fuel. Now LEFT...%d\n",fuel);
    pthread_mutex_unlock(&mutexFuel);
}


int main(){

    pthread_t th[5];
    int i;
    pthread_cond_init(&condFuel,NULL);
    pthread_mutex_init(&mutexFuel,NULL);
    for (i = 0; i < 5; i++){
        int *a = malloc(sizeof(int));
        *a = i;
        if(i == 4){
            if(pthread_create(&th[i] , NULL, &fuelGas, a) != 0) // a ile argüman gönderiyoruz arg = a oluyor gibi düşün
                return 1;
            }
        else
            if(pthread_create(&th[i] , NULL, &car, a) != 0) // a ile argüman gönderiyoruz arg = a oluyor gibi düşün
                return 2;
    }

    for (i = 0; i < 5; i++){
        int* r;
        if(pthread_join(th[i], (void**) &r) != 0){// &r ile geri dönüş değerini alıyoruz

            return 3;
        }
    }
    pthread_cond_destroy(&condFuel);
    pthread_mutex_destroy(&mutexFuel);

}

*/

/*
int primes[10] = {2,3,5,7,11,13,17,19,23,29};

void *sumprime(void *arg){

    int * toplam = malloc(sizeof(int));
    int i,ilkyari = 0,sonyari = 0;

    if(*(int*)arg == 0){
        *(int*)arg = primes[0];
        for(i = 0;i < ((sizeof(primes)/sizeof(int))/2); i++){
            ilkyari = ilkyari + primes[i];
        }
    }
    else{
        for(i = ((sizeof(primes)/sizeof(int))/2); i <(sizeof(primes)/sizeof(int)); i++ ){
            sonyari = sonyari + primes[i];
        }
    }
    *(toplam )= sonyari + ilkyari;
    printf("%d ",*toplam);
    return (void*)toplam; //her ikisi de bir geri dönüş veriyor
}

int main(){

    pthread_t th[2];
    int i;

    for (i = 0; i < 2; i++){
        int *a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i] , NULL, &sumprime, a) != 0)
            return 1;

    }
    int globalsum = 0;
    for (i = 0; i < 2; i++){
        int* r;
        if(pthread_join(th[i], (void**) &r) != 0){// &r ile geri dönüş değerini alıyoruz

            return 2;
        }

          globalsum += *r;
          free(r);
    }
    printf("%d",globalsum);

}


*/

/*
//threadlerde race conditions olduğu için genel olarak her bir threadin farklı adresler için çalıştığından emin ol

int primes[10] = {2,3,5,7,11,13,17,19,23,29};

void *find(void *arg){
    int index = *(int*)arg;//voidi herhangi bir değere atayamadığımı için başta tür dönüşümü yapıyoruz.
    printf("%d ",primes[index]);
    free(arg);
}

//BU KODU TAM ANLAMADIM BİR DAHA BAK
// VOİD* her türden adresi alabilir UNUTMA BUNU HER YERDE KULLANILIYOR
int main(){

    pthread_t th[10];
    int i;

    for (i = 0; i < 10; i++){
        int *a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i] , NULL, &find, a "parametre olarak gönderiyoruz") != 0)
            return 1;

    }
    for (i = 0; i < 10; i++){
        if(pthread_join(th[i], NULL) != 0)
            return 2;
    }
}
*/

/*
void *routine()
{
    int value = (rand() % 6) + 1;
    int * result = (int *)malloc(sizeof(int));
    *result = value; //burada adresini alsa zaten bu adres temizleneceği için undefined behavior oluyor.
    printf("result = %p\n",result);
    return (void*)result;

}

int main()
{
    srand(time(NULL));
    pthread_t th;
    int * res;
    if (pthread_create(&th, NULL, &routine, NULL) != 0)
    {
        return 1;
    }
    if (pthread_join(th, (void**) &res) != 0) //res değerine diğer taraftaki dönüş değeri atanıyor
    {
        return 2;
    }
    printf("res = %p\n",res);
    printf("value = %d",*res);
}



*/

/*

int main(){

    int pid = fork();
    printf("hello ");

    if(pid == -1){
        return 1;
    }

    printf("process id %d\n",getpid());

    if(pid != 0){
        wait(NULL);

    }


}





*/

// THREADLER VOID* DÖNDÜRÜYOR DİKKAT ET

/*pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void* myturn(void * arg ){
    int i = 0;
    int *iptr = (int*)malloc(sizeof(int));
    *iptr = 5;
    for(i = 0; i < 100000; i++){

        pthread_mutex_lock(&lock);
        printf("my turn %d %d  \n", i, *iptr);
        (*iptr)++;
        pthread_mutex_unlock(&lock);
    }
    return iptr;
}

void* yourturn(){
    int i;
    for(i = 0; i < 100000; i++){
        pthread_mutex_lock(&lock);
        printf("your turn\n");
        pthread_mutex_unlock(&lock);
    }
}

int main(){
    pthread_t newthread;
    int *result;


    pthread_create(&newthread,NULL, myturn, NULL);
    yourturn();
    pthread_join(newthread, (void*)&result);

    printf("done v = %d\n", *result);
}
*/