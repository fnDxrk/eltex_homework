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

    // Инициализация состояния для чёрных
    if (str_clock->current_player == 0) {
        str_clock->current_player = 2;
    }

    // Если ход не чёрных, то выход
    if (str_clock->current_player != 2) {
        printf("Сейчас не ваш ход.\n");
        return EXIT_FAILURE;
    }

    // Текущее время
    time_t start_time = time(NULL);

    while (1) {
        // Цикл ожидания, если не твой ход
        while (str_clock->current_player != 2) {
            sleep(1);
        }

        // Проверка, сколько времени прошло
        time_t current_time = time(NULL);
        str_clock->black_time = current_time - start_time;

        // Если время привысело максимальное время, то поражение
        if (str_clock->black_time > MAX_TIME) {
            printf("Техническое поражение чёрных.\n");
            break;
        }

        // Вывод времени
        printf("Время чёрных : %ld секунд\n", str_clock->black_time);

        // Переход к следующему ходу (для белых)
        str_clock->current_player = 1;

        sleep(1);
    }

    // Отключаемся от разделяемой памяти
    if (shmdt(str_clock) == -1) {
        perror("shmdt");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
