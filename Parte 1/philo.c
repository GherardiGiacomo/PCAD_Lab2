#include "my_semaphore.c"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUMERO_FILOSOFI 5
#define TOT_MANGIATO 5

my_semaphore bacchette[NUMERO_FILOSOFI];
my_semaphore sala_attesa;

int mangiato[NUMERO_FILOSOFI] = {0}; // Array di contatori, serve per contare quante volte ha mangiato ogni filosofo

void* filosofo(void* n) {
    int id = *(int*)n;
    for (int i = 0; i < TOT_MANGIATO; i++) {
        usleep((rand() % 3 + 1) * 1000000); //il filosofo pensa tra 1 e 3 secondi
        printf("Filosofo %d: sta pensando\n", id);
        
        my_sem_wait(&sala_attesa);
        my_sem_wait(&bacchette[id]);
        my_sem_wait(&bacchette[(id+1)%NUMERO_FILOSOFI]);

        printf("Filosofo %d: ha la sua bacchetta sinistra\n", id);
        printf("Filosofo %d: ha la sua bacchetta destra\n", id);
        printf("\nFilosofo %d: sta mangiando\n\n", id);
        
        usleep(1000000); //faccio "mangiare" il filosofo per 1 secondo

        mangiato[id]++;
        printf("\nFilosofo %d: ha mangiato %d volte\n\n", id, mangiato[id]); // Stampo il numero di volte che il filosofo ha mangiato
        
        my_sem_signal(&bacchette[id]);
        my_sem_signal(&bacchette[(id+1)%NUMERO_FILOSOFI]);
        my_sem_signal(&sala_attesa);

        printf("Filosofo %d: ha rilasciato le sue due bacchette\n", id);
        }
        my_sem_signal(&bacchette[id]);
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t filosofi[NUMERO_FILOSOFI];
    int id[NUMERO_FILOSOFI];

    my_sem_init(&sala_attesa, NUMERO_FILOSOFI - 1);

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

    for(int i=0; i<NUMERO_FILOSOFI; i++){
        my_sem_destroy(&bacchette[i]);
    }
    return 0;
}