#include "spider.h"

int ft_strlen(char *str)
{
    int i = 0;

    if (!str)
        return (0);
    while (str[i])
        i++;
    return (i);
}

int ft_strlen_2D(char **str)
{
    int i = 0;

    if (!str)
        return (0);
    while (str[i])
        i++;
    return (i);
}

void    usage(void)
{
    fprintf(stderr, "Usage: ./spider [-rlp] URL\n"
                    "\tOption -r : recursively downloads the images in a URL received as a parameter.\n"
                    "\tOption -r -l [N] : indicates the maximum depth level of the recursive download.\n"
                    "\tIf not indicated, it will be 5.\n"
                    "\tOption -p [PATH] : indicates the path where the downloaded files will be saved.\n"
                    "\tIf not specified, ./data/ will be used.\n");
}

bool    err_msg(const char *err_msg)
{
    fprintf(stderr, "%s", err_msg);
    return false;
}

int ft_strcmp(char *str1, char *str2)
{
    int i = 0;

    while ((str1[i] && str2[i]) && (str1[i] == str2[i]))
        i++;
    return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

int ft_atoi (char *str)
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

void    print_data(t_spider data)
{
    printf("\n\n");
    printf("hostname = %s\n", data.hostname);
    printf("pathName = %s\n", data.pathName);
    printf("deepness = %d\n", data.deepness);
    printf("b pathName = %d\n", data.pathNameSelected);
    printf("b deepness = %d\n", data.deepnessSelected);
    printf("b https = %d\n", data.https);
    printf("\n\n");
}

// la fonction ne free pas
char    *strjoin(char *dest, char *src, bool free_dest)
{
    char *str = calloc((ft_strlen(dest) + ft_strlen(src) + 1), sizeof(char));
    if (!str)
    {
        fprintf(stderr, "Error malloc, strjoin\n");
        return (NULL);
    }
    if (dest)
        strcpy(str, dest);
    if (free_dest)
        ft_free(dest);
    strcat(str, src);
    return (str);
}

// cherche to_find dans str et renvoie l'index de sa position (premiere occurance)
int strstr_index(char *str, char *to_find)
{
    int i;
    int j;

    i = 0;
    while (str[i])
    {
        if (str[i] == to_find[0])
        {
            j = 0;
            while ((str[i] && to_find[j]) && str[i] == to_find[j])
            {
                if (to_find[j + 1] == 0)
                    return (i - j + 1);
                i++;
                j++;
            }
            continue;
        }
        i++;
    }
    return (0);
}

void    ft_free(void *ptr)
{
    free(ptr);
    ptr = NULL;
}