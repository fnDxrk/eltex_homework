#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

// Обработчик SIGINT
void sigint_handler(int signum) {
    printf("Поток %ld получил SIGINT\n", (long)pthread_self());
}

// Функция для первого потока: блокирует все сигналы
void* thread_block_signals(void* arg) {
    printf("Поток 1 (блокировка сигналов) запущен\n");

    sigset_t mask;
    sigfillset(&mask);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    while (1) {
        sleep(1);
    }

    return NULL;
}

// Функция для второго потока: принимает SIGINT с помощью обработчика
void* thread_handle_sigint(void* arg) {
    printf("Поток 2 (обработка SIGINT) запущен\n");

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    pthread_sigmask(SIG_UNBLOCK, &mask, NULL);

    while (1) {
        sleep(1);
    }

    return NULL;
}

// Функция для третьего потока: принимает SIGQUIT с помощью sigwait
void* thread_handle_sigquit(void* arg) {
    printf("Поток 3 (обработка SIGQUIT) запущен\n");

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);

    int sig;
    while (1) {
        sigwait(&mask, &sig);
        if (sig == SIGQUIT) {
            printf("Поток 3 получил SIGQUIT\n");
            break;
        }
    }

    printf("Поток 3 завершен\n");
    return NULL;
}

int main() {
    pthread_t threads[3];

    pthread_create(&threads[0], NULL, thread_block_signals, NULL);
    pthread_create(&threads[1], NULL, thread_handle_sigint, NULL);
    pthread_create(&threads[2], NULL, thread_handle_sigquit, NULL);

    pthread_join(threads[2], NULL);

    printf("Основной поток завершен\n");
    return 0;
}
