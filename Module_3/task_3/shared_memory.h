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
    time_t white_time; // Время белых
    time_t black_time; // Время чёрных
    int current_turn; // 0 - белые, 1 -  чёрные
} ChessClock;

// Подключение к разделяемой памяти
ChessClock* connect_shared_memory();

// Отключение от разделяемой памяти
int disconnect_shared_memory(ChessClock* clock);

#endif
