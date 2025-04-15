#include "spider.h"

bool    get_links(t_spider *data)
{
    // occurence sur ahref

    char    *start_navbar = data->html_page;
    char    *end_navbar;
    char    *occ = start_navbar;
    char    buffer[500];
    int     link_len;

    // est ce qu'il y a plusieurs navbar ?
    while ((start_navbar = strstr(start_navbar, "<nav")))
    {
        end_navbar = strstr(start_navbar, "</nav>");
        if (!end_navbar)
            return (err_msg("no end balise of nav founded\n"));
        // chercher toute les pages dedans
        while ((occ = strstr(occ, "href=\"")) && occ < end_navbar)
        {
            link_len = 0;
            occ+= 6;
            while (occ[link_len] && occ[link_len] != '\"')
                link_len++;
            if (link_len < 500)
            {
                int i = 0;
                while (occ[i] && i < link_len)
                {
                   buffer[i] = occ[i];
                   i++;
                }
                buffer[i] = 0;
                if (checkDouble(data->links_name_tab, buffer))
                {
                    ft_parse_links(buffer);
                    int links_name_tab_len = ft_strlen_2D(data->links_name_tab);
                    data->links_name_tab = realloc(data->links_name_tab, sizeof(char *) * (links_name_tab_len + 2));
                    if (!data->links_name_tab)
                        return (err_msg("Error syscall, realloc, links name\n"));
                    data->links_name_tab[links_name_tab_len] = 0;
                    data->links_name_tab[links_name_tab_len + 1] = 0;
                    // allocate the new links for the table
                    data->links_name_tab[links_name_tab_len] = malloc(sizeof(char) * (link_len + 1));
                    if (!data->links_name_tab[links_name_tab_len])
                       return (err_msg("Error syscall, malloc, links name\n"));
                    strcpy(data->links_name_tab[links_name_tab_len], buffer);
                }
                else
                {
                    occ++;
                    continue;
                }
            }
            else
            {
                occ++;
                continue;
            }
            occ++;
        }
        start_navbar++;
        end_navbar++;
    }
    return (true);
}