#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_TIME 15 // Максимальное время

typedef struct {
    time_t white_time; // Время для белых
    time_t black_time; // Время для черных
    int current_player; // Чей текущий код
} Chess_clock;

int main(int argc, char* argv[])
{
    // Генерация ключа
    key_t shmkey = ftok("keyfile", 2);
    if (shmkey == -1) {
        perror("ftok");
        exit(1);
    }

    // Создание разделяемой памяти
    int shmid = shmget(shmkey, sizeof(Chess_clock), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Подключение к разделяемой памяти
    Chess_clock* str_clock = (Chess_clock*)shmat(shmid, NULL, 0);
    if (str_clock == (Chess_clock*)-1) {
        perror("shmat");
        return EXIT_FAILURE;
    }

    // Отключаемся от разделяемой памяти
    if (shmdt(str_clock) == -1) {
        perror("shmdt");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
