#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    /* Дескриптор FIFO*/
    int fd_fifo;
    /* Текст для файла FIFO */
    char fifo_message[] = "Простой текста для файла FIFO";
    /* Буфер для хранения текста */
    char buffer[100];

    /* Названия файла FIFO */
    const char* fifo_filename = "fifo_file";

    /* Удаляем файл FIFO, если он существует */
    unlink(fifo_filename);

    /* Создаём FIFO */
    if ((mkfifo(fifo_filename, 0666)) == -1) {
        fprintf(stderr, "Невозможно создать файл FIFO\n");
        exit(EXIT_FAILURE);
    }

    /* Открываем FIFO для записи и чтения */
    if ((fd_fifo = open(fifo_filename, O_RDWR)) == -1) {
        fprintf(stderr, "Невозможно открыть файл FIFO\n");
        exit(EXIT_FAILURE);
    }

    /* Записываем текст в FIFO */
    if (write(fd_fifo, fifo_message, strlen(fifo_message)) == -1) {
        fprintf(stderr, "Невозможно записать в файл FIFO\n");
        exit(EXIT_FAILURE);
    }

    /* Читаем текст из FIFO */
    if (read(fd_fifo, buffer, sizeof(buffer)) == -1) {
        fprintf(stderr, "Невозможно прочитать файл FIFO\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Вывод файла FIFO :\n%s\n", buffer);
    }

    /* Закрываем файл FIFO */
    close(fd_fifo);

    return 0;
}
