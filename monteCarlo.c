#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
// Antônio Costa Satiro de Souza 10723636

long acertosTotais = 0;
pthread_mutex_t mutex_carlo = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    unsigned int seed;
    long pontos;
} ArgumentosThreads;

void* threadMonteCarlo(void* argumentos) {
    ArgumentosThreads *args = (ArgumentosThreads *)argumentos;
    long acerto = 0;
    unsigned int seed = args->seed;

    for (long i = 0; i < args->pontos; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX; // geração de números aleatórios dividido pelo maior aleatório possível para gerar valor entre [0,1]
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0)
            acerto++;
    }

    pthread_mutex_lock(&mutex_carlo);
    acertosTotais += acerto;
    pthread_mutex_unlock(&mutex_carlo);
    return NULL;
}

int main(int argc, char *argv[]) {
    struct timespec inicio, fim;
    long pontosTotal = atol(argv[1]);
    int nThreads = atol(argv[2]);
    pthread_t threads[nThreads];
    ArgumentosThreads argumentos[nThreads];
    long pontosPorThread = pontosTotal / nThreads;
    unsigned int seedBase = (unsigned int)time(NULL);
    clock_gettime(CLOCK_MONOTONIC, &inicio); // extração do tempo começo da parte paralela

    for(int i = 0; i < nThreads; i++){ // alocação de atributos pra cada thread e criação das threads
        argumentos[i].seed = seedBase + i * 41u; // para garantir que todo thread tenha números diferetes se modifica a seed com um número primo (41) e o u para indicar com unsigned
        if(i == nThreads - 1){
            argumentos[i].pontos = pontosTotal - (i * pontosPorThread);
        } else {
            argumentos[i].pontos = pontosPorThread;
        }
        pthread_create(&threads[i], NULL, threadMonteCarlo, &argumentos[i]);
    }

    for(int i = 0; i < nThreads; i++) pthread_join(threads[i], NULL); // espera do retorno das threads
    clock_gettime(CLOCK_MONOTONIC, &fim); // extração do tempo fim da parte paralela
    double tempo = (fim.tv_sec - inicio.tv_sec) +
    (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    double pi = 4.0 * (double)acertosTotais / (double)pontosTotal;
    printf("pi ≈ %.8f\n", pi); 
    printf("[Método de Monte carlo] N threads: %.2i | Tempo: %.4f s\n", nThreads, tempo);
    return 0;
}