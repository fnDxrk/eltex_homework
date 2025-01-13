#include "shared_memory.h"

// Функция для неблокирующего ввода
int is_enter_pressed() {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) == 1;
}

// Вывод сообщения о поражении
void handle_game_over(ChessClock* clock, int player) {
    if (clock->loser == PLAYER_WHITE) {
        printf("Техническое поражение белых.\n");
    } else if (clock->loser == PLAYER_BLACK) {
        printf("Техническое поражение чёрных.\n");
    }

    cleanup_shared_memory();
}

// Основная логика для игрока
void run_player(ChessClock* clock, int player) {
    const char* player_name = (player == PLAYER_WHITE) ? "белых" : "чёрных";
    time_t* player_time = (player == PLAYER_WHITE) ? &clock->white_time : &clock->black_time;

    while (clock->loser == -1) { // Пока игра не завершена
        if (clock->current_turn == player) {
            printf("Ход %s. Таймер запущен...\n", player_name);

            while (clock->current_turn == player && clock->loser == -1) {
                (*player_time)++;
                printf("Прошло %ld секунд...\n", *player_time);
                sleep(1);

                // Проверка на превышение лимита времени
                if (*player_time >= TIME_LIMIT) {
                    clock->loser = player;
                    clock->current_turn = GAME_OVER;
                    handle_game_over(clock, player);
                    return;
                }

                // Проверка нажатия Enter для передачи хода
                if (is_enter_pressed()) {
                    getchar();
                    printf("Ход %s завершён. Ожидание хода противника...\n", player_name);
                    *player_time = 0; // Обнуляем время текущего игрока
                    clock->current_turn = (player == PLAYER_WHITE) ? PLAYER_BLACK : PLAYER_WHITE;
                    break;
                }
            }
        } else {
            printf("Ожидание хода противника...\n");
            while (clock->current_turn != player && clock->loser == -1) {
                sleep(1);
            }
        }
    }

    // Если игра завершена, выводим сообщение о поражении
    if (clock->loser != -1) {
        handle_game_over(clock, player);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <0 для белых, 1 для чёрных>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int player = atoi(argv[1]);
    if (player != PLAYER_WHITE && player != PLAYER_BLACK) {
        fprintf(stderr, "Некорректный игрок. Используйте 0 для белых или 1 для чёрных.\n");
        return EXIT_FAILURE;
    }

    ChessClock* clock = connect_shared_memory();
    if (clock == NULL) {
        fprintf(stderr, "Ошибка! Не удалось подключиться к разделяемой памяти.\n");
        return EXIT_FAILURE;
    }

    run_player(clock, player);

    disconnect_shared_memory(clock);
    return EXIT_SUCCESS;
}
