#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#define MSGQUEUE_KEY 1234
#define SEMAPHORE_KEY 5678

// Объединение для semctl
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    // Создаем очередь сообщений
    int msgid = msgget(MSGQUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Создаем семафор
    int semid = semget(SEMAPHORE_KEY, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализируем семафор значением 1 (игрок 1 начинает первым)
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    printf("Очередь сообщений и семафоры созданы.\n");

    return 0;
}
