#include "spider.h"

bool    ft_find_double_names(char *image_name_temp, t_spider *data)
{
    int i = 0;

    if (!data->img_name_tab)
        return (true);
    while (data->img_name_tab[i])
    {
        if (!strcmp(image_name_temp, data->img_name_tab[i]))
            return (false);
        i++;
    }
    return (true);
}

bool    check_double_image_before_allocate(t_spider *data, int occ, int i, int mimes_size)
{
    char image_name_temp[2000];

    int images_name_size = occ - i + mimes_size;
    if (images_name_size > 1999)
        return (fprintf(stderr, "Error: Image name is too long\n"), false);
    ft_strncpy(image_name_temp, data->html_page + i, images_name_size);
    if (!ft_find_double_names(image_name_temp, data))
        return (false);
    return (true);
}

bool    find_images_names(t_spider *data, int occ, int mimes_size)
{
    int i = occ;
    static int img_name_tab_size = 0;
    
    while (i >= 0 && data->html_page[i - 1] && (data->html_page[i - 1] != '"' && data->html_page[i - 1] != ','))
        i--;

    int images_name_size = occ - i + mimes_size;
    if (!check_double_image_before_allocate(data, occ, i, mimes_size))
        return (true);
    data->img_name_tab = realloc(data->img_name_tab, sizeof(char *) * (img_name_tab_size + 2));
    if (!data->img_name_tab)
    {
        fprintf(stderr, "Error realloc: find images names\n");
        return (false);
    }
    data->img_name_tab[img_name_tab_size + 1] = NULL;
    data->img_name_tab[img_name_tab_size] = malloc(sizeof(char) * (images_name_size + 1));
    if (!data->img_name_tab[img_name_tab_size])
    {
        fprintf(stderr, "Error malloc: find images names\n");
        return (false);
    }
    ft_strncpy(data->img_name_tab[img_name_tab_size], data->html_page + i, images_name_size);
    img_name_tab_size++;
    return (true);
}

bool    find_images_mimes(t_spider *data, const char *mimes, int mime_length)
{
    char *occ;

    occ = data->html_page;
    while ((occ = strstr(occ, mimes)) != NULL)
    {
        printf("occ trouve\n");
        if (!find_images_names(data, occ - data->html_page, mime_length))
            return (false);
        occ++;
    }
    return (true);
}

bool    find_images(t_spider *data)
{
   // faire la recherche avec jpg
    if (!find_images_mimes(data, ".jpg", 4))
        return (false);
    if (!find_images_mimes(data, ".jpeg", 5))
        return (false);
    if (!find_images_mimes(data, ".gif", 4))
        return (false);
    if (!find_images_mimes(data, ".png", 4))
        return (false);
    if (!find_images_mimes(data, ".bmp", 4))
        return (false);

    if (!data->img_name_tab)
        return (fprintf(stderr, "no images found\n\n answear:\n\n%s\n", data->html_page), true);
        
    int i = 0;
    while (data->img_name_tab[i])
    {
        fprintf(stderr, "image name [%d] = %s\n", i, data->img_name_tab[i]);
        i++;
    }
    return (true);
}