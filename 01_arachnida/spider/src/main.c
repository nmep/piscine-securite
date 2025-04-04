#include "spider.h"

// ./spider -r URL
// ./spider -r -l [N]
// ./spider -p [PATH]
// ./spider -r -l [N] -p [PATH] URL

void    free_data(t_spider *data)
{
    free(data->hostname);
    free(data->html_page);
}

char    *get_hostName(char *url)
{
    int i = 0;
    char *hostname;

    while (url[i] && url[i] != '/')
        i++;
    if (url[i] == 0)
        return (NULL);
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

bool    Url_to_Hostname(t_spider *data, char **av, int ac)
{
    int http, https;

    https = strncmp("https://", av[ac - 1], 8);
    http = strncmp("http://", av[ac - 1], 7);

    if (http != 0 && https != 0)
    {
        fprintf(stderr, "URL error: No http(s) found in the last argument\n");
        return (false);
    }
    if (http == 0)
    {
        av[ac - 1] += 7;
        data->hostname = get_hostName(av[ac - 1]);
        if (!data->hostname)
            return (false);
        av[ac - 1] -= 7;
    }
    else if (https == 0)
    {
        data->https = true;
        av[ac - 1] += 8;
        data->hostname = get_hostName(av[ac - 1]);
        if (!data->hostname)
            return (false);
        av[ac - 1] -= 8;
    }
    return (true);
}

bool    init_data(t_spider *data, char **av, int ac)
{
    memset(data, 0, sizeof(*data));
    data->deepness = 5;
    ft_strcpy("../data", data->pathName);
    if (!Url_to_Hostname(data, av, ac))
        return (false);
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

    if (!init_socket(&data))
    {
        free_data(&data);
        return (2);
    }

    // scrappping du site

    free_data(&data);
    return (0);
}