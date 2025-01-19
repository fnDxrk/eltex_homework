#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <unistd.h>

#define MSGQUEUE_KEY 1234
#define SEMAPHORE_KEY 5678

// Структура для сообщений
struct msg_buffer {
    long msg_type;
    char move[10];
};

// Объединение для semctl
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Функция для ожидания семафора
void wait_semaphore(int semid) {
    struct sembuf sb = {0, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

// Функция для освобождения семафора
void signal_semaphore(int semid) {
    struct sembuf sb = {0, 1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <номер игрока (1 или 2)>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int player_number = atoi(argv[1]);
    if (player_number != 1 && player_number != 2) {
        printf("Номер игрока должен быть 1 или 2.\n");
        exit(EXIT_FAILURE);
    }

    // Инициализация очереди сообщений
    int msgid = msgget(MSGQUEUE_KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Инициализация семафора
    int semid = semget(SEMAPHORE_KEY, 1, 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Массив с ходами партии (пример из известной партии)
    const char *moves[] = {
        "e2e4", "e7e5", 
        "g1f3", "b8c6",
        "f1b5", "a7a6",
        "b5a4", "g8f6",
        "e1g1", "exit"
    };
    
    int move_index = 0;  // Индекс текущего хода

    struct msg_buffer message;

    if (player_number == 1) {
        // Игрок 1 начинает первым
        while (1) {
            wait_semaphore(semid);
            strcpy(message.move, moves[move_index]);
            message.msg_type = 2;  // Отправляем игроку 2

            printf("Игрок 1 сходил: %s\n", message.move);

            if (msgsnd(msgid, &message, sizeof(message.move), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }

            if (strcmp(message.move, "exit") == 0) {
                break;
            }

            move_index++;
            signal_semaphore(semid);

            // Ждем ответа от игрока 2
            if (msgrcv(msgid, &message, sizeof(message.move), 1, 0) == -1) {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }

            printf("Игрок 2 сходил: %s\n", message.move);

            if (strcmp(message.move, "exit") == 0) {
                break;
            }
        }
    } else {
        // Игрок 2 ждет хода от игрока 1
        while (1) {
            // Ждем хода от игрока 1
            if (msgrcv(msgid, &message, sizeof(message.move), 2, 0) == -1) {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }

            printf("Игрок 1 сходил: %s\n", message.move);

            if (strcmp(message.move, "exit") == 0) {
                break;
            }

            wait_semaphore(semid);

            strcpy(message.move, moves[move_index]);
            message.msg_type = 1;  // Отправляем игроку 1

            printf("Игрок 2 сходил: %s\n", message.move);

            if (msgsnd(msgid, &message, sizeof(message.move), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }

            if (strcmp(message.move, "exit") == 0) {
                break;
            }

            move_index++;
            signal_semaphore(semid);
        }
    }

    printf("Игра завершена.\n");

    return 0;
}
