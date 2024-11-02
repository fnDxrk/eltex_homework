#include <math.h>
#include <stdio.h>

#define NUMBER_2 2

int main()
{
    int n = 0;
    double result = 0;

    printf("Введите натуральное число N: ");
    scanf("%d", &n);
    if (n < 1) {
        printf("Ошибка: Введите натуральное число больше 0.\n");
        return 1;
    }

    result = pow(NUMBER_2, n);

    printf("2 ^ %d = %0.f\n", n, result);

    return 0;
}
