import time
import threading

def contagem_pesada(n):
    while n > 0:
        n -= 1

if __name__ == "__main__":
    n = 100_000_000
    inicio = time.time()
    # Cria duas threads, cada uma faz metade do trabalho (1 contagem pesada)
    t1 = threading.Thread(target=contagem_pesada, args=(n,))

    t2 = threading.Thread(target=contagem_pesada, args=(n,))
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    fim = time.time()
    print(f"Tempo com Threads: {fim - inicio:.4f} segundos")