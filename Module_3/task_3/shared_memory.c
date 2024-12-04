#include "shared_memory.h"

ChessClock* connect_shared_memory()
{
    // Генерация ключа
    key_t shmkey = ftok(SHM_FILE, ID);
    if (shmkey == -1) {
        perror("ftok");
        return NULL;
    }

    // Создание разделяемой памяти
    int shmid = shmget(shmkey, sizeof(ChessClock), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return NULL;
    }

    // Подключение к разделяемой памяти
    ChessClock* clock = (ChessClock*)shmat(shmid, NULL, 0);
    if (clock == (ChessClock*)-1) {
        perror("shmat");
        return NULL;
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
