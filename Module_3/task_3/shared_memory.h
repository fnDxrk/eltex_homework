#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Файл для генерации ключа
#define SHM_FILE "keyfile"

// ID для генерации ключа
#define ID 2

// Порог времени
#define TIME_LIMIT 60

typedef struct {
    time_t last_white_move_time; // Время последнего хода белых
    time_t last_black_move_time; // Время последнего хода черных
    time_t white_total_time; // Суммарное время белых
    time_t black_total_time; // Суммарное время черных
    int current_turn; // 0 - ход белых, 1 - ход черных
    int time_limit_exceeded; // Флаг для технического поражения
} ChessClock;

// Подключение к разделяемой памяти
ChessClock* connect_shared_memory();

// Отключение от разделяемой памяти
int disconnect_shared_memory(ChessClock* clock);

#endif
