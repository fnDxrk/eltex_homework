#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define SHM_FILE "keyfile"  // Файл для генерации ключа
#define ID 2                // ID для генерации ключа
#define TIME_LIMIT 5        // Порог времени
#define PLAYER_WHITE 0      // Игрок белые
#define PLAYER_BLACK 1      // Игрок чёрные
#define GAME_OVER -1        // Конец игры

typedef struct {
    time_t white_time;  // Время белых
    time_t black_time;  // Время чёрных
    int loser;          // Кто проиграл: -1 - никто, 0 - белые, 1 - чёрные
    int current_turn;   // Чей ход: 0 - белые, 1 - чёрные
} ChessClock;

// Подключение к разделяемой памяти
ChessClock* connect_shared_memory();

// Отключение от разделяемой памяти
int disconnect_shared_memory(ChessClock* clock);

// Очистка разделяемой памяти и ключа
void cleanup_shared_memory();

#endif
