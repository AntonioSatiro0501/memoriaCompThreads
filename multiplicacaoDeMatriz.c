#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
// Antônio Costa Satiro de Souza 10723636

typedef struct{ // struct para passagem de argumentos de cada thread
    long *A, *B, *C;
    long colunas, linhaInicial, linhaFinal;
} ArgumentosThreads;

void* multiplicacaoLinhas(void* argumentos){ 
    ArgumentosThreads *args = (ArgumentosThreads *)argumentos;
    long colunas = args->colunas;
    for (long i = args->linhaInicial; i < args->linhaFinal; i++) { // loop determinado pelas linhas definidas previamente na criação de threads
        for (long j = 0; j < colunas; j++) {
            long soma = 0;
            for (long k = 0; k < colunas; k++) {
                soma += args->A[i * colunas + k] * args->B[k * colunas + j];
            }
            args->C[i * colunas + j] = soma;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){

    long tamanho = atol(argv[1]);
    long *A = malloc(tamanho * tamanho * sizeof(long));
    long *B = malloc(tamanho * tamanho * sizeof(long));
    long *C = malloc(tamanho * tamanho * sizeof(long));
    struct timespec inicio, fim;

    for(long i = 0; i < tamanho * tamanho; i++){ // definição aleatória dos elementos de A e B
        A[i] = rand() % 50;
        B[i] = rand() % 50;
    }

    int nThreads = atol(argv[2]);
    pthread_t threads[nThreads];
    ArgumentosThreads argumentos[nThreads];
    int espacoLeitura = tamanho / nThreads;
    clock_gettime(CLOCK_MONOTONIC, &inicio); // extração do tempo começo da parte paralela
    
    for(int i = 0; i < nThreads; i++){ // alocação de atributos pra cada thread e criação das threads
        argumentos[i].A = A;
        argumentos[i].B = B;
        argumentos[i].C = C;
        argumentos[i].colunas = tamanho;
        argumentos[i].linhaInicial = i * espacoLeitura;
        if(i == nThreads - 1){
            argumentos[i].linhaFinal = tamanho;
        } else {
            argumentos[i].linhaFinal = (i + 1) * espacoLeitura;
        }
        pthread_create(&threads[i], NULL, multiplicacaoLinhas, &argumentos[i]);
    }


    for(int i = 0; i < nThreads; i++) pthread_join(threads[i], NULL); // espera do retorno das threads
    free(A); 
    free(B); 
    free(C);
    clock_gettime(CLOCK_MONOTONIC, &fim); // extração do tempo fim da parte paralela
    double tempo = (fim.tv_sec - inicio.tv_sec) +
    (fim.tv_nsec - inicio.tv_nsec) / 1e9; 
    printf("[Multiplicação de Matrizes] N threads: %.2i | Tempo: %.4f s\n", nThreads, tempo);
    return 0;
}
