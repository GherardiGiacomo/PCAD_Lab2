#include "my_semaphore.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUMERO_FILOSOFI 5
#define TOT_MANGIATO 5

my_semaphore bacchette[NUMERO_FILOSOFI];
my_semaphore sala_attesa;
my_semaphore sala_mutex;

int n_sala[NUMERO_FILOSOFI] = {0};
int mangiato[NUMERO_FILOSOFI] = {0}; // Array di contatori, serve per contare quante volte ha mangiato ogni filosofo

void* filosofo(void* n) {
    int id = *(int*)n;
    for (int i = 0; i < TOT_MANGIATO; i++) {
        usleep((rand() % 3 + 1) * 1000000); //il filosofo pensa tra 1 e 3 secondi
        printf("Filosofo %d: sta pensando\n", id);
        
        my_sem_wait(&sala_attesa);
        n_sala[id]=1;
        my_sem_wait(&bacchette[id]);
        my_sem_wait(&bacchette[(id+1)%NUMERO_FILOSOFI]);

        printf("Filosofo %d: ha preso le bacchette e sta mangiando\n", id);
        
        usleep(1000000); //faccio "mangiare" il filosofo per 1 secondo

        mangiato[id]++;
        printf("Filosofo %d: ha mangiato %d volte\n", id, mangiato[id]); // Stampo il numero di volte che il filosofo ha mangiato
        
        my_sem_signal(&bacchette[id]);
        my_sem_signal(&bacchette[(id+1)%NUMERO_FILOSOFI]);
        
        my_sem_wait(&sala_mutex);
        n_sala[id]=0; //il filosofo esce nella sala d'attesa
        printf("\nLa sala d'attesa e' attualmente composta dai filosofi: \n");
        for(int i=0; i< NUMERO_FILOSOFI; i++){
            if(n_sala[i] == 1){
                printf("%d ", i);
                }
            }
        printf("\n\n"); 
        my_sem_signal(&sala_mutex);
        my_sem_signal(&sala_attesa);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t filosofi[NUMERO_FILOSOFI];
    int id[NUMERO_FILOSOFI];

    my_sem_init(&sala_attesa, NUMERO_FILOSOFI - 1);
    my_sem_init(&sala_mutex, 1);

    for (int i = 0; i < NUMERO_FILOSOFI; i++) {
        my_sem_init(&bacchette[i], 1);
    }
    for(int i=0; i<NUMERO_FILOSOFI; i++){
        id[i] = i;
        pthread_create(&filosofi[i], NULL, filosofo, &id[i]);
    }

    for (int i = 0; i < NUMERO_FILOSOFI; i++) {
        pthread_join(filosofi[i], NULL);  //aspetta che il filosofo i-esimo abbia finito di mangiare
    }
    my_sem_destroy(&sala_attesa);
    my_sem_destroy(&sala_mutex);

    for(int i=0; i<NUMERO_FILOSOFI; i++){
        my_sem_destroy(&bacchette[i]);
    }
    return 0;
}