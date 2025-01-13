#include "shared_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

ChessClock* connect_shared_memory()
{
    // Создание keyfile, если он не существует
    int fd = open(SHM_FILE, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open (keyfile creation)");
        return NULL;
    }
    close(fd); // Закрываем файл, так как он нам больше не нужен

    // Генерация ключа
    key_t shmkey = ftok(SHM_FILE, ID);
    if (shmkey == -1) {
        perror("ftok");
        return NULL;
    }

    // Создание разделяемой памяти
    int shmid = shmget(shmkey, sizeof(ChessClock), IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) {
        // Если память уже существует, подключаемся к ней
        if (errno == EEXIST) {
            shmid = shmget(shmkey, sizeof(ChessClock), 0666);
            if (shmid == -1) {
                perror("shmget (existing)");
                return NULL;
            }
        } else {
            perror("shmget (new)");
            return NULL;
        }
    }

    // Подключение к разделяемой памяти
    ChessClock* clock = (ChessClock*)shmat(shmid, NULL, 0);
    if (clock == (ChessClock*)-1) {
        perror("shmat");
        return NULL;
    }

    // Инициализация структуры только при первом создании
    if (shmid != -1 && errno != EEXIST) {
        clock->current_turn = 0;
        clock->black_time = 0;
        clock->white_time = 0;
        clock->loser = -1;
    }

    return clock;
}

int disconnect_shared_memory(ChessClock* clock)
{
    if (shmdt(clock) == -1) {
        perror("shmdt");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void cleanup_shared_memory()
{
    // Проверяем, существует ли файл
    if (access(SHM_FILE, F_OK) == -1) {
        printf("Файл %s уже удалён.\n", SHM_FILE);
        return;
    }

    // Генерация ключа
    key_t shmkey = ftok(SHM_FILE, ID);
    if (shmkey == -1) {
        perror("ftok (cleanup)");
        return;
    }

    // Получение идентификатора разделяемой памяти
    int shmid = shmget(shmkey, sizeof(ChessClock), 0666);
    if (shmid == -1) {
        perror("shmget (cleanup)");
        return;
    }

    // Удаление разделяемой памяти
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl (cleanup)");
        return;
    }

    // Удаление keyfile
    if (unlink(SHM_FILE) == -1) {
        perror("unlink (cleanup)");
        return;
    }

    printf("Разделяемая память и keyfile очищены.\n");
}
