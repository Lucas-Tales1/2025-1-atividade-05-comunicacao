#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];
int ready = 0;  // 0 = vazio, 1 = escrito, 2 = lido

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* writer_thread(void* arg) {
    pthread_mutex_lock(&mutex);
    strcpy(buffer, "Olá, comunicação via arquivo simulada com threads!");
    printf("Escritor: Mensagem escrita no buffer.\n");
    ready = 1;
    pthread_cond_signal(&cond);

    // Espera o leitor sinalizar que leu
    while (ready != 2) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    printf("Escritor: Finalizado.\n");
    return NULL;
}

void* reader_thread(void* arg) {
    pthread_mutex_lock(&mutex);

    // Espera o escritor escrever
    while (ready != 1) {
        pthread_cond_wait(&cond, &mutex);
    }

    printf("Leitor: Mensagem lida: %s\n", buffer);

    // Sinaliza que terminou a leitura
    ready = 2;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    printf("Leitor: Finalizado.\n");
    return NULL;
}

int main() {
    pthread_t writer, reader;

    pthread_create(&writer, NULL, writer_thread, NULL);
    pthread_create(&reader, NULL, reader_thread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    return 0;
}
