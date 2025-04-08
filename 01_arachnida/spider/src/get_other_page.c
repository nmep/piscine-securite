#include "spider.h"

bool    get_links(t_spider *data)
{
    // occurence sur ahref

    char *start_navbar = data->html_page;
    char *occ;

    // est ce qu'il y a plusieurs navbar ?

    while ((start_navbar = strstr(start_navbar, "<nav")))
    {
        printf("nav bar trouve a %ld %.10s\n", start_navbar - data->html_page, start_navbar);
        // chercher toute les pages dedans
        while ((occ = strstr(start_navbar, "<a href")))
        start_navbar++;
    }
    return (true);
}