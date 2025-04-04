#include "spider.h"

static bool parse_deepness(t_spider *data, char **av, int i)
{
    if (data->deepnessSelected)
    {
        if (is_digit(av[i]))
        {
            // eviter les doublons de propositions de profondeur
            if (data->deepness != 5)
            {
                fprintf(stderr, "Error [%s]: Do not suggest more then one deepness\n", av[i]);
                return (false);
            }
            data->deepness = ft_atoi(av[i]);
        }
        else
        {
            fprintf(stderr, "Error invalid arg %s, default value a keeping\n", av[i]);
            perror("Stat Error");
        }
    }
    return true;
}

static bool parse_pathName(t_spider *data, char **av, int i)
{
    static int p = 0;

    if (data->pathNameSelected)
    {
        if (is_dir(av[i]))
        {
            if (!access(av[i], R_OK | W_OK))
            {
                if (ft_strlen(av[i]) > 1023)
                {
                    perror("access\n");
                    fprintf(stderr, "The name of your directory %s is too long 1023 char max\n", av[i]);
                    return (false);
                }
                if (p >= 1)
                {
                    fprintf(stderr, "Error [%s]: Do not suggest more then one PathName\n", av[i]);
                    return (false);
                }
                ft_strcpy(av[i], data->pathName);
                p++;
            }
        }
    }
    return (true);
}

static bool    handle_option_arg(char **av, int i, t_spider *data)
{
    if (!parse_pathName(data, av, i))
        return (false);
    if (!parse_deepness(data, av, i))
        return (false);
    return (true);
}

static void parse_options(char **av, int i, int j, t_spider *data)
{
    // r
    if (data->recursive == false && av[i][j] == 'r')
        // mettre la recursivite a true
        data->recursive = true;
    // p
    else if (data->pathNameSelected == false && av[i][j] == 'p')
        // mettre pathnameselected a true
        data->pathNameSelected = true;

    // l
    else if (data->pathNameSelected == false && av[i][j] == 'l')
        // mettre depnessSelected a true
        data->deepnessSelected = true;
}

static bool    check_option(char **av, t_spider *data)
{
    int i = 0;
    int j;

    while (av[i])
    {
        if (av[i][0] == '-')
        {
            j = 1;
            while (av[i][j])
            {
                if (av[i][j] != 'r' && av[i][j] != 'l' && av[i][j] != 'p')
                {
                    fprintf(stderr, "Option Error: %c is not a recognized option\n", av[i][j]);
                    return (false);
                }
                parse_options(av, i, j, data);
                j++;
            }
        }
        else
        {
            if (!handle_option_arg(av, i, data))
                return (false);
        }
        i++;
    }
    return (true);
}

bool    arg_pars(char **av, t_spider *data)
{
    // int i = 0;

    // check les options (leurs positions et si oui ou non elles sont valide 
    // attention au doublon)
    (void)data;
    if (!check_option(av, data))
        return (false);
    return (true);
}