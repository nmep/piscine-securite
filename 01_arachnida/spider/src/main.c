#include "spider.h"

// ./spider -r URL
// ./spider -r -l [N]
// ./spider -p [PATH]
// ./spider -r -l [N] -p [PATH] URL

void    free_data(t_spider *data)
{
    int i = -1;

    close(data->site_fd);
    if (data->hostname)
        free(data->hostname);
    if (data->html_page)
        free(data->html_page);
    if (data->img_name_tab)
    {
        while (data->img_name_tab[++i])
            free(data->img_name_tab[i]);
        free(data->img_name_tab);
    }
    if (data->links_name_tab)
    {
        i = -1;
        while (data->links_name_tab[++i])
            free(data->links_name_tab[i]);
        free(data->links_name_tab);
    }
}

char    *get_hostName(char *url)
{
    int i = 0;
    char *hostname;

    while (url[i] && url[i] != '/')
        i++;
    if (url[i] == 0) {
        fprintf(stderr, "Error Url: no '/' found at the end [%s]\n", url);
        return (NULL);
    }
    hostname = malloc(sizeof(char) * (i + 1));
    if (!hostname)
    {
        fprintf(stderr, "Error malloc, hostname\n");
        return (NULL);
    }
    for (int j = 0; url[i] && j < i; j++)
        hostname[j] = url[j];
    hostname[i] = 0;
    return (hostname);
}

bool    Url_to_Hostname(t_spider *data, char *url)
{
    int http, https;

    https = strncmp("https://", url, 8);
    http = strncmp("http://", url, 7);

    if (http != 0 && https != 0)
    {
        fprintf(stderr, "URL error: No http(s) found in the last argument\n");
        return (false);
    }
    if (http == 0)
    {
        url += 7;
        data->hostname = get_hostName(url);
        if (!data->hostname)
            return (false);
        url -= 7;
    }
    else if (https == 0)
    {
        data->https = true;
        url += 8;
        data->hostname = get_hostName(url);
        if (!data->hostname)
            return (false);
        url -= 8;
    }
    return (true);
}

bool    init_data(t_spider *data, char **av, int ac)
{
    memset(data, 0, sizeof(*data));
    data->deepness = 5;
    ft_strcpy("./data", data->pathName);
    if (!Url_to_Hostname(data, av[ac - 1]))
        return (false);
    if (strlen(av[ac - 1]) > 1024)
        return (err_msg("The url is to long, it must be lesser then 1024\n"));
    strcpy(data->url, av[ac - 1]);
    return (true);
}

int main(int ac, char **av)
{
    t_spider data;
    if (ac < 2 || ac > 7)
    {
        usage();
        return (2);
    }

    ac -= 1;
    if (!init_data(&data, ++av, ac))
    {
        usage();
        return (2);
    }

    if (!arg_pars(av, &data))
    {
        free_data(&data);
        return (2);
    }

    print_data(data);

    if (!scrapper(&data))
    {
        free_data(&data);
        return (2);
    }

    // scrappping du site

    free_data(&data);
    return (0);
}

// lance le programme j'ai modif les links pour avoir que le path
// il faut maintenant faire des requetes avec jusu'a ce que ca marche parmis 
// tout les path que j'ai, je garde le fd que j'ai enregistre au debut
// est ce que j'ai besoin de mettre un slash devant ?
// il faut refactore pour que ca puisse envoyer assez de requete