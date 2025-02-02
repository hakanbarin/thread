// FONKSİYONLARDAN GERİ DÖNÜŞ DEĞERİ OLARAK ADRES ALIRSAK HATA ALIRIZ ÇÜNKÜ FONKSİYON BİTİNCE
// DEĞİŞKENLER DEALLOCATED EDİLİYOR AŞIRI ÖNEMLİ DİKKAT ET HEAPTE YER AÇIP (MALLOC İLE) ONU DÖNDÜRÜRSEK
// SORUN KALMAZ


// void* değerini bir değişkene atayamadığımız için int* yaptıktan sonra değişken dönüşümü uyguluyoruz.!!!!!!!! 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <nl_types.h>
#include <time.h>
pthread_mutex_t mutex;
pthread_mutex_t mutex2;

void *routine(void *arg){
    if(pthread_mutex_trylock(&mutex) == 0){ //eğer geri dönüş değeri 0 sa kitleyebiliyor farklı bir şeyse kitleyemiyor
        printf("LOCK\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
    }
    else{
        printf("didn't lock\n");
    }
}


int main(){

    pthread_t th[2];
    int i;
    pthread_mutex_init(&mutex,NULL);
    for (i = 0; i < 2; i++){
        int *a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i] , NULL, &routine, a) != 0) // a ile argüman gönderiyoruz arg = a oluyor gibi düşün
            return 1;
            sleep(2);
        
    }
    
    for (i = 0; i < 2; i++){
        int* r;
        if(pthread_join(th[i], (void**) &r) != 0){// &r ile geri dönüş değerini alıyoruz
        
            return 2;
        }
    }
    pthread_mutex_destroy(&mutex);
    
}























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