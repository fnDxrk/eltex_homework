#include "shared_memory.h"

void handle_game_over(ChessClock* clock)
{
    if (clock->loser == PLAYER_WHITE) {
        printf("Техническое поражение белых.\n");
    } else if (clock->loser == PLAYER_BLACK) {
        printf("Техническое поражение чёрных.\n");
    }
}

int cleanup_and_exit(ChessClock* clock)
{
    if (disconnect_shared_memory(clock) == EXIT_FAILURE) {
        fprintf(stderr, "Ошибка! Не удалось отключиться от разделяемой памяти.\n");
    }
    return EXIT_SUCCESS;
}

int main()
{
    ChessClock* clock = connect_shared_memory();
    if (clock == NULL) {
        fprintf(stderr, "Ошибка! Не удалось подлкючиться к разделяемой памяти.");
        return EXIT_FAILURE;
    }

    clock->current_turn = PLAYER_BLACK;

    if (clock->current_turn != PLAYER_BLACK) {
        printf("Сейчас ход белых!\n");
        cleanup_and_exit(clock);
    }

    printf("Ход чёрных. Таймер запущен...\n");
    sleep(1);

    while (clock->current_turn != GAME_OVER) {
        clock->black_time += 1;
        printf("Прошло %ld секунд...\n", clock->black_time);
        sleep(1);

        if (clock->black_time >= TIME_LIMIT) {
            printf("Техническое поражение чёрных.\n");
            clock->loser = PLAYER_BLACK;
            clock->current_turn = GAME_OVER;
            return EXIT_SUCCESS;
        }

        if (clock->current_turn == PLAYER_WHITE) {
            printf("Ход белых. Ожидание...\n");
            while (clock->current_turn == PLAYER_WHITE && clock->current_turn != GAME_OVER) {
                sleep(1);
            }
        }

        if (clock->loser != GAME_OVER) {
            break;
        }
    }

    handle_game_over(clock);
    cleanup_and_exit(clock);
}
