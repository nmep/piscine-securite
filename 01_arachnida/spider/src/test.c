#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int ac, char **av)
{
    const char href_template[11] = "href=\"http";
    char *occ = av[1];

    // trouver un href
    if (!(occ = strstr(occ, "href=\"")))
        return 2;
    // check si le href est une page
    // si ca commence directement par http / https alors c'est une page et je dois tout prendre a partir du 3eme / recontre
    int j = 0;
    bool    http_link = false;

    while ((occ[j] && href_template[j]) && (occ[j] == href_template[j]))
        j++;
    http_link = (href_template[j] == 0);

    (http_link) ? printf("le liens est http\n") : printf("le lien n'est pas http\n");

    if (http_link)
    {
        int counter = 0;
        j = 6;
        while (occ[j] && counter < 3 && occ[j] != '\'')
        {
            if (occ[j] == '/')
                counter++;
            j++;
        }
        if (counter == 3)
        {
            j--;
            int http_link_len = 0;
            while (occ[j] && occ[j++] != '"')
                http_link_len++;
            printf("le liens fait %d de longueur\n", http_link_len);
        }
    }
    
    // si ca commence par un / alors je prend tout
}