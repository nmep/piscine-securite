#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void    ft_strncpy(char *dest, char *src, int n)
{
    int i = 0;
    for (; src[i] && i < n; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = 0;
    printf("strncpy i = %d\n", i);
}

int main(int ac, char **av)
{
    char name[2000];
    int occ = strstr(av[1], ".jpeg") - av[1];
    int i = occ;

    while (i >= 0 && av[1][i - 1] && av[1][i - 1] != '"')
    i--;
    
    printf("occ = %d char = %c | i = %d char = %c\n", occ, av[1][occ], i, av[1][i]);
    int images_name_size = occ - i + 5;


    printf("image name size = %d char = %c\n", images_name_size, (av[1] + i)[images_name_size]);
    ft_strncpy(name, av[1] + i, images_name_size);

    printf("le nom est %s\n", name);
}