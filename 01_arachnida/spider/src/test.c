#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int ac, char **av)
{
    printf("len = %lu\n", strlen(av[1]));
    // si ca commence par un / alors je prend tout
}