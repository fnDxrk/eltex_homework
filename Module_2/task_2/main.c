#include <math.h>
#include <stdio.h>

#define NUMBER_2 2

int power_of_two(int n, int* result)
{
    *result = (int)pow(NUMBER_2, n);

    return *result;
}

int main()
{
    int n = 0;
    int result = 0;

    printf("Введите натуральное число N: ");
    scanf("%d", &n);
    if (n < 1) {
        printf("Ошибка! Введите натуральное число больше 0.\n");
        return 1;
    }

    power_of_two(n, &result);

    printf("2 ^ %d = %d\n", n, result);

    return 0;
}
