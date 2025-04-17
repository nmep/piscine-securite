#include "spider.h"



bool    https_skip_image_header(BIO *bio, int bytesRead, int image_fd)
{
    char buffer[16384];
    char *start_image = NULL;
    int header_len;

    if ((bytesRead = BIO_read(bio, buffer, sizeof(buffer) - 1)) == -1)
    {
        fprintf(stderr, "BIO is NULL. Failed to create SSL connection.\n");
        ERR_print_errors_fp(false);
        return (false);
    }
    buffer[bytesRead] = 0;

    printf("buffer = [%s]\n", buffer);
    sleep(1);
    if ((start_image = strstr((const char *) buffer, "\r\n\r\n")) == NULL)
        return (fprintf(stderr, "no \\r\\n\\r\\n found in header"), false);
    start_image += 4;

    header_len = start_image - buffer;
    write(image_fd, start_image, bytesRead - header_len);
    return (true);
}

// bool    bio_read(t_spider *data, BIO *bio)
// {
//     char buffer[16384];
//     int bytes_reads;

//     while ((bytes_reads = BIO_read(bio, buffer, sizeof(buffer) - 1)) > 0)
//     {
//         buffer[bytes_reads] = 0;
//         data->html_page = strjoin(data->html_page, buffer, true);
//         if (!data->html_page)
//         return (false);
//     }
//     if (!get_links(data))
//         return (false);
//     if (!find_images(data))
//         return (false);
//     return (true);
// }

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
        // lire
        // avancer jusqu'au body
        // boucle de read et write
    }
    else
    {

    }
    return (true);
}