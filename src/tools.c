#include "spider.h"

void    usage(void)
{
    fprintf(stderr, "Usage: ./spider [-rlp] URL\n"
                    "\tOption -r : recursively downloads the images in a URL received as a parameter.\n"
                    "\tOption -r -l [N] : indicates the maximum depth level of the recursive download.\n"
                    "\tIf not indicated, it will be 5.\n"
                    "\tOption -p [PATH] : indicates the path where the downloaded files will be saved.\n"
                    "\tIf not specified, ./data/ will be used.\n");
}

int ft_strcmp(char *str1, char *str2)
{
    int i = 0;

    while ((str1[i] && str2[i]) && (str1[i] == str2[i]))
        i++;
    return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

int atoi (char *str)
{
    int i = 0;
    int signe = 1;
    int res = 0;

    while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
        i++;
    while (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            signe *= -1;
        i++;
    }
    while (str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        res = (res * 10) + (str[i] - 48);
        i++;
    }
    return (res * signe);
    
}

void    ft_strcpy(char *src, char *dest)
{
    int i = 0;

    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
}

int ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
}


bool is_digit(char *str)
{
    int i = 0;

    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (false);
        i++;
    }
    return (true);
}

bool is_dir(char *pathName)
{
    struct stat buffer;

    if (stat(pathName, &buffer) == -1)
        return (false);
    if (buffer.st_mode & __S_IFDIR)
        return (true);
    return (false);
}