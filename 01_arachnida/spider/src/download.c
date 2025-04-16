#include "spider.h"

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
    // get name
    char *fileName = NULL;

    fileName = ft_get_file_name(data, i);
    if (!fileName)
        return (false);
    // open
    data->img_fd = open(fileName, O_CREAT | W_OK, 0644);
    if (data->img_fd == -1)
        return (free(fileName), fprintf(stderr, "Error: open %s\n", strerror(errno)), false);
    free(fileName);
    return (true);
}

bool    request_to_get_image(t_spider *data, struct addrinfo *rp, struct addrinfo *result, int i)
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
        return (err_msg("Failed to send\n"), close(data->site_fd), false);
    return (true);
}

bool    ft_iterative_download(t_spider *data, int sfd)
{
    unsigned char    buff[4096];
    int     bytesRead = 0;

    (void)data;
    while ((bytesRead = recv(sfd, buff, 4095, 0)) > 0)
    {
        buff[bytesRead] = 0;
        printf("bytes read = %d\n", bytesRead);
        printf("buff = [%s]\n", buff);
        // ecrire directement dans le fichier        
    }
    if (bytesRead == -1)
        return (err_msg("Error recv image\n"), false);
    return (true);
}

bool    ft_recursive_download(t_spider *data)
{
    (void)data;
    return (true);
}
