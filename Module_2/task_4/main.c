#include <stdalign.h>
#include <stdio.h>

struct Person {
    char name[30];
    char surname[50];
    int age;
    double gpa;
};

struct CompressedPerson {
    char name[10];
    char surname[20];
    char age;
    float gpa;
};

int main()
{
    struct Person person;
    struct CompressedPerson compressed_person;

    printf("Size struct Person: %ld\n", sizeof(person));
    printf("Size struct CompressedPerson: %ld\n", sizeof(compressed_person));
    printf("\n");
    printf("Alignof struct Person: %ld\n", alignof(person));
    printf("Alignof struct CompressedPerson: %ld\n", alignof(compressed_person));

    return 0;
}
