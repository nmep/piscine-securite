#include "spider.h"

// ./spider -r URL
// ./spider -r -l [N]
// ./spider -p [PATH]
// ./spider -r -l [N] -p [PATH] URL

int main(int ac, char **av)
{
    t_spider data;
    if (ac < 2 || ac > 7)
    {
        usage();
        return (2);
    }

    // init
    bzero(&data, sizeof(data));
    data.deepness = 5;
    ft_strcpy("../data", data.pathName);
    // parsing
    if (!arg_pars(++av, &data))
        return (2);
    // ping de l'url
    // scrappping du site

    printf("b recursive = %d\n", data.recursive);
    printf("b deepness = %d\n", data.deepnessSelected);
    printf("b Path = %d\n", data.pathNameSelected);

    printf("v deepness = %d\n", data.deepness);
    printf("v pathName = %s\n", data.pathName);

    return (0);
}