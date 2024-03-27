#include "my_semaphore.h"

int my_sem_init(my_semaphore *ms, unsigned int v){ //inizializza i campi del semaforo
    ms->V = v;
    pthread_mutex_init(&ms->lock, NULL);
    pthread_cond_init(&ms->varcond, NULL);
    return 0;
}

int my_sem_wait(my_semaphore *ms){//decrementa il valore del semaforo
    pthread_mutex_lock(&ms->lock);
    while (ms->V == 0) {
        pthread_cond_wait(&ms->varcond, &ms->lock);
    }
    ms->V--;
    pthread_mutex_unlock(&ms->lock);
    return 0;
}

int my_sem_signal(my_semaphore *ms){ //incrementa il valore del semaforo
    pthread_mutex_lock(&ms->lock);
    ms->V++;
    pthread_cond_signal(&ms->varcond);
    pthread_mutex_unlock(&ms->lock);
    return 0;
}

int my_sem_destroy(my_semaphore *ms){ //libera i campi (chiamando le funzione pthread_mutex_destroy e pthread_cond_destroy)
    pthread_mutex_destroy(&ms->lock);
    pthread_cond_destroy(&ms->varcond);
    return 0;
}

/*int main(){ 
    my_semaphore ms;
        my_sem_init(&ms,1);
        my_sem_wait(&ms);
        my_sem_signal(&ms);
        my_sem_destroy(&ms);    
    return 0;
}*/