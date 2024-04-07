#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int N = 10; // Numero di passeggeri
int C = 5; // Posti nel bus
int count = 0; // Contatore dei passeggeri nel bus

pthread_mutex_t mutex_bus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_passeggero = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_bus_pieno = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_bus_vuoto = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_bus_ritornato = PTHREAD_COND_INITIALIZER;

void *bus (void *arg) {
    while (1){
        pthread_mutex_lock(&mutex_bus);
        while(count < C)
            pthread_cond_wait(&cond_bus_pieno, &mutex_bus);
        printf("\n\nIl bus e' pieno, partiamo!\n\n");
        usleep(1000000);
        printf("\nIl bus e' ritornato \n");
        pthread_cond_broadcast(&cond_bus_ritornato);
        count = 0;
        pthread_cond_broadcast(&cond_bus_vuoto);
        pthread_mutex_unlock(&mutex_bus);
    }
    return NULL;
}

void *passenger(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex_passeggero);
        while(count == C)
            pthread_cond_wait(&cond_bus_vuoto, &mutex_passeggero);
        count++;
        printf("\nPasseggero %d e' salito sul bus\n", *(int*)arg);
        if (count == C)
            pthread_cond_signal(&cond_bus_pieno);
        pthread_mutex_unlock(&mutex_passeggero);

        pthread_mutex_lock(&mutex_passeggero);
        pthread_cond_wait(&cond_bus_ritornato, &mutex_passeggero);
        printf("\nPasseggero %d e' sceso dal bus\n", *(int*)arg);
        pthread_mutex_unlock(&mutex_passeggero);

        usleep(1000000); // Tempo di attesa prima del controllo dei posti
    }
    return NULL;
}

int main(){
    pthread_t bus_thread;
    pthread_t passenger_thread[N];
    int id[N];
    pthread_create(&bus_thread, NULL, bus, NULL);
    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&passenger_thread[i], NULL, passenger, &id[i]);
    }
    pthread_join(bus_thread, NULL);
    for (int i = 0; i < N; i++) {
        pthread_join(passenger_thread[i], NULL);
    }
    return 0;
}