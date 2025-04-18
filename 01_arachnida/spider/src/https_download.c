#include "spider.h"

bool    https_skip_image_header(BIO *bio, int bytesRead, int image_fd)
{
    char buffer[16384];
    char *start_image = NULL;
    int header_len;

    if ((bytesRead = BIO_read(bio, buffer, sizeof(buffer) - 1)) == -1)
    {
        fprintf(stderr, "BIO is NULL. Failed to create SSL connection.\n");
        ERR_print_errors_fp(stderr);
        return (false);
    }
    buffer[bytesRead] = 0;

    if ((start_image = strstr((const char *) buffer, "\r\n\r\n")) == NULL)
        return (fprintf(stderr, "no \\r\\n\\r\\n found in header"), false);
    start_image += 4;
    header_len = start_image - buffer;
    write(image_fd, start_image, bytesRead - header_len);
    return (true);
}

void    ft_https_recursive_download(BIO *bio, int image_fd)
{
    char buffer[16384];
    int bytesRead = 0;
    
    if ((bytesRead = BIO_read(bio, buffer, 16384)) == -1)
    {
        fprintf(stderr, "BIO is NULL. Failed to create SSL connection.\n");
        ERR_print_errors_fp(stderr);
        return ;
    }
    write(image_fd, buffer, bytesRead);
    if (bytesRead > 0)
        ft_https_recursive_download(bio, image_fd);
    return ;
}

void    ft_https_iterative_download(BIO *bio, int image_fd)
{
    char buffer[16384];
    int bytesRead = 0;

    while ((bytesRead = BIO_read(bio, buffer, 16384)) > 0)
        write(image_fd, buffer, bytesRead);

    if (bytesRead == -1)
    {
        fprintf(stderr, "BIO is NULL. Failed to create SSL connection.\n");
        ERR_print_errors_fp(stderr);
        return ;
    }
    return ;
}

bool    https_request_to_get_image(t_spider *data, BIO *bio, int i)
{
    int bytesRead = 0;

    if (!bio)
    {
        fprintf(stderr, "BIO is NULL. Failed to create SSL connection.\n");
        ERR_print_errors_fp(stderr);
        return (true);
    }
    if (data->recursive)
    {
        if (!bio_write_to_url(data, bio, data->img_name_tab[i]))
            return (false);
        if (!https_skip_image_header(bio, bytesRead, data->img_fd))
            return (false);
        ft_https_recursive_download(bio, data->img_fd);
    }
    else
    {
        if (!bio_write_to_url(data, bio, data->img_name_tab[i]))
           return (false);
        if (!https_skip_image_header(bio, bytesRead, data->img_fd))
            return (false);
        ft_https_iterative_download(bio, data->img_fd);
    }
    return (true);
}