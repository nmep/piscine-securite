#include "spider.h"

/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         HTTP                                #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */


static  char    *ft_get_file_name(t_spider *data, int i)
{
    char *fileName = NULL;
    char *fileName_and_Path = NULL;

    int j = strlen(data->img_name_tab[i]) - 1;
    int len_img_name = 1;

    while (data->img_name_tab[i][j] && data->img_name_tab[i][j] != '/')
    {
        j--;
        len_img_name++;
    }
    fileName = malloc(len_img_name + 1);
    if (!fileName)
        return (err_msg("Error malloc: FileName\n"), NULL);

    int k = 0;
    while (data->img_name_tab[i][j])
    {
        fileName[k] = data->img_name_tab[i][j];
        j++;
        k++;
    }
    fileName[k] = 0;
    fileName_and_Path = strjoin(data->pathName, fileName, false);
    if (!fileName_and_Path)
        return (free(fileName), err_msg("Error join malloc: FileNameAndPath\n"), NULL);
    free(fileName);
    return (fileName_and_Path);
}

bool    ft_openfile_in_dir(t_spider *data, int i)
{
    data->img = ft_get_file_name(data, i);
    if (!data->img)
        return (false);

    data->img_fd = open(data->img, O_CREAT | W_OK, 0644);
    if (data->img_fd == -1)
        return (free(data->img), fprintf(stderr, "Error: open %s\n", strerror(errno)), true);
    free(data->img);
    return (true);
}

bool    http_request_to_get_image(t_spider *data, struct addrinfo *rp, struct addrinfo *result, int i)
{
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        data->site_fd = socket(rp->ai_family, rp->ai_socktype, 
        rp->ai_protocol);
        if (data->site_fd == -1) {
            fprintf(stderr, "Socket error data->site_fd = -1\n");
            continue;
        }
        if (connect(data->site_fd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;
        close(data->site_fd);
    }
    if (rp == NULL) {
        fprintf(stderr, "Could not connect\n");
        close(data->site_fd);
        return (false);
    }
    if (!send_request(data, data->site_fd, data->img_name_tab[i]))
        return (err_msg("Failed to send\n"), close(data->site_fd), true);
    return (true);
}

bool    skip_image_header(int sfd, int bytesRead, int image_fd)
{
    char buff[4096];
    char *start_image = NULL;
    int header_len;

    if ((bytesRead = recv(sfd, buff, 4095, 0)) == -1)
        return (fprintf(stderr, "Error recv: %s\n", strerror(errno)), false);
    buff[bytesRead] = 0;
    
    if ((start_image = strstr((const char *) buff, "\r\n\r\n")) == NULL)
        return (fprintf(stderr, "no \\r\\n\\r\\n found in header"), true);
    start_image += 4;

    header_len = start_image - buff;
    write(image_fd, start_image, bytesRead - header_len);
    return (true);
}

bool    ft_http_iterative_download(t_spider *data, int sfd)
{
    unsigned char    buff[4096];
    int     bytesRead = 0;

    if (!skip_image_header(sfd, bytesRead, data->img_fd))
        return (false);
    while ((bytesRead = recv(sfd, buff, 4096, 0)) > 0)
    {
        printf("tour de boucle\n");
        write(data->img_fd, buff, bytesRead);
    }
    if (bytesRead == -1)
        return (fprintf(stderr, "Error recv: %s\n", strerror(errno)), false);
    return (true);
}

bool    ft_http_recursive_download(t_spider *data, int sfd, int n_read)
{
    unsigned char    buff[4096];
    int     bytesRead = 0;

    printf("n read = %d\n", n_read);
    if (n_read == 0)
    {
        if (!skip_image_header(sfd, bytesRead, data->img_fd))
            return (false);
    }
    if ((bytesRead = recv(sfd, buff, 4096, 0)) == -1)
        return (fprintf(stderr, "Error recv: %s\n", strerror(errno)), false);
    if (bytesRead > 0)
    {
        printf("lecture n %d br = %d\n", n_read, bytesRead);
        write(data->img_fd, buff, bytesRead);
        ft_http_recursive_download(data, sfd, n_read + 1);
    }
    printf("lecture n %d j'ecrit = %d char\n", n_read, bytesRead);
    return (true);
}



