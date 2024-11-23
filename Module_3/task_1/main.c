#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Названия файла FIFO */
const char* fifo_filename = "fifo_file";

/* Создание FIFO */
void create_fifo()
{
    /* Удаляем файл FIFO, если он существует */
    unlink(fifo_filename);

    if ((mkfifo(fifo_filename, 0666)) == -1) {
        fprintf(stderr, "Невозможно создать файл FIFO\n");
        exit(EXIT_FAILURE);
    }
}

/* Открываем FIFO для записи и чтения */
int open_fifo()
{
    int fd_fifo = open(fifo_filename, O_RDWR);
    if (fd_fifo == -1) {
        fprintf(stderr, "Невозможно открыть файл FIFO\n");
        exit(EXIT_FAILURE);
    }

    return fd_fifo;
}

/* Запись текста в FIFO */
void write_fifo(int fd_fifo, const char* message)
{
    if (write(fd_fifo, message, strlen(message)) == -1) {
        fprintf(stderr, "Невозможно записать в файл FIFO\n");
        exit(EXIT_FAILURE);
    }
}

/* Чтение текста из FIFO */
void read_fifo(int fd_fifo)
{
    char buffer[100];

    if (read(fd_fifo, buffer, sizeof(buffer)) == -1) {
        fprintf(stderr, "Невозможно прочитать файл FIFO\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Вывод файла FIFO :\n%s\n", buffer);
    }
}

/* Обработчик сигнала */
void signal_fifo(int signal)
{
    if (signal == SIGABRT) {
        int fd_fifo = open_fifo();
        char signal_message[] = "Получен сигнал SIGABRT";
        write_fifo(fd_fifo, signal_message);
    }
}

int main(int argc, char** argv)
{
    /* Дескриптор FIFO */
    int fd_fifo;

    create_fifo();
    fd_fifo = open_fifo();

    /* Устанавливаем обработчик для сигнала SIGABRT */
    if (signal(SIGABRT, signal_fifo) == SIG_ERR) {
        fprintf(stderr, "Не удалось установить обработчик для SIGABRT\n");
        exit(EXIT_FAILURE);
    }

    /* Генерируем сигнал SIGABRT */
    raise(SIGABRT);

    read_fifo(fd_fifo);

    close(fd_fifo);

    return 0;
}
