#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
// Antônio Costa Satiro de Souza 10723636

typedef struct{ // struct para passagem de argumentos de cada thread
    float *A, *B, *C;
    long inicio, fim;
} ArgumentosThreads;

void* somaVetores(void* argumentos){ // função para realizar a soma dos vetores A e B em C por meio de threads
    ArgumentosThreads *args = (ArgumentosThreads *)argumentos;
    for(long i = args->inicio; i < args->fim; i++)
        args->C[i] = args->A[i] + args->B[i];
    return NULL;
}

int main(int argc, char *argv[]){

    long tamanho = atol(argv[1]);
    float *A = malloc(tamanho * sizeof(float)); // alocação dinámica dos vetores
    float *B = malloc(tamanho * sizeof(float));
    float *C = malloc(tamanho * sizeof(float));
    struct timespec inicio, fim;

    for(long i = 0; i < tamanho; i++){ // definição aleatória dos elementos de A e B
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
        argumentos[i].inicio = i * espacoLeitura;
        if(i == nThreads - 1){
            argumentos[i].fim = tamanho;
        } else {
            argumentos[i].fim = (i + 1) * espacoLeitura;
        }
        pthread_create(&threads[i], NULL, somaVetores, &argumentos[i]);
    }


    for(int i = 0; i < nThreads; i++) pthread_join(threads[i], NULL); // espera do retorno das threads
    free(A); 
    free(B); 
    free(C);
    clock_gettime(CLOCK_MONOTONIC, &fim); // extração do tempo fim da parte paralela
    double tempo = (fim.tv_sec - inicio.tv_sec) +
    (fim.tv_nsec - inicio.tv_nsec) / 1e9; 
    printf("[Soma Vetores Gigantescos] N threads: %.2i | Tempo: %.4f s\n", nThreads, tempo);
    return 0;
}
