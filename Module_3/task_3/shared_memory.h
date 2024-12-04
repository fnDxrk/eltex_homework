#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SHM_FILE "keyfile"
#define ID 2

typedef struct {
    time_t white_time; // Время белых
    time_t black_time; // Время чёрных
    int player; // Чей ход : 0 - чёрные, 1 - белые
} ChessClock;

ChessClock* connect_shared_memory();

int disconnect_shared_memory(ChessClock* clock);

#endif
