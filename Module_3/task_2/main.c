#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    /* Если это второй запуск */
    if (argc > 1) {
        printf("%s\n", argv[1]);
        return 0;
    }

    /* Выводим PID процесса */
    printf("PID : %d\n", getpid());

    /* Ждём 1 секунду */
    sleep(1);

    /* Массив аргументов для exec */
    char* args_exec[] = { argv[0], "Hello World", NULL };

    /* Выполняем тот же процесс, но с массивом аргументов */
    execv(argv[0], args_exec);

    return 0;
}
