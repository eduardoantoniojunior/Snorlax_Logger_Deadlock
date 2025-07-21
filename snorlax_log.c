#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Declara dois mutexes globais para simular locks
pthread_mutex_t L1;
pthread_mutex_t L2;

// Função que retorna o timestamp atual em microssegundos
long long get_timestamp_us() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (long long)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

// Converte timestamp Unix em microssegundos para string legível
char* convert_unix_us_to_string(long long microseconds) {
    static char final_str[80];  // buffer persistente entre chamadas

    time_t seconds = microseconds / 1000000;
    long micros = microseconds % 1000000;

    struct tm *tm_info = localtime(&seconds);

    char datetime[64];
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", tm_info);

    snprintf(final_str, sizeof(final_str), "%s.%06ld", datetime, micros);

    return final_str;
}

// Função auxiliar que grava os eventos no arquivo trace.log
void log_lock(FILE* log_file, const char* thread, const char* lock, const char* acao) {
    long long ts = get_timestamp_us();
    fprintf(log_file, "%s | %s | %s | %s\n", thread, acao, lock, convert_unix_us_to_string(ts));
    fflush(log_file);
}

// Função executada pela Thread 1 (L1 -> L2)
void* thread_1(void* arg) {
    const char* id = "Thread 1";
    FILE* log_file = fopen("trace.log", "a");

    log_lock(log_file, id, "L1", "TENTANDO.");
    pthread_mutex_lock(&L1);
    log_lock(log_file, id, "L1", "ADQUIRIDO");

    log_lock(log_file, id, "L2", "TENTANDO.");
    pthread_mutex_lock(&L2);
    log_lock(log_file, id, "L2", "ADQUIRIDO");

    pthread_mutex_unlock(&L2);
    log_lock(log_file, id, "L2", "LIBERADO.");

    pthread_mutex_unlock(&L1);
    log_lock(log_file, id, "L1", "LIBERADO.");

    fclose(log_file);
    return NULL;
}

// Função executada pela Thread 2 (L2 -> L1)
void* thread_2(void* arg) {
    const char* id = "Thread 2";
    FILE* log_file = fopen("trace.log", "a");

    log_lock(log_file, id, "L2", "TENTANDO.");
    pthread_mutex_lock(&L2);
    log_lock(log_file, id, "L2", "ADQUIRIDO");

    log_lock(log_file, id, "L1", "TENTANDO.");
    pthread_mutex_lock(&L1);
    log_lock(log_file, id, "L1", "ADQUIRIDO");

    pthread_mutex_unlock(&L1);
    log_lock(log_file, id, "L1", "LIBERADO.");

    pthread_mutex_unlock(&L2);
    log_lock(log_file, id, "L2", "LIBERADO.");

    fclose(log_file);
    return NULL;
}

// Função principal
int main() {
    pthread_t t1, t2;

    // Limpa log anterior
    FILE* log_file = fopen("trace.log", "w");
    fclose(log_file);

    pthread_mutex_init(&L1, NULL);
    pthread_mutex_init(&L2, NULL);

    pthread_create(&t1, NULL, thread_1, NULL);
    pthread_create(&t2, NULL, thread_2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&L1);
    pthread_mutex_destroy(&L2);

    printf("[INFO] Execução finalizada. Veja trace.log para os eventos.\n");
    return 0;
}
