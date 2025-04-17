#include "spider.h"

bool    send_request(t_spider *data, int sfd, char *path)
{
    char request[1024];
    if (snprintf(request, sizeof(request),
            "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "\r\n", path, data->hostname) <= 0)
    {
            fprintf(stderr, "Error syscall, snprintf failed add hostname\n");
            return (false);
    }
    if (send(sfd, request, ft_strlen(request), 0) == -1)
    {
        fprintf(stderr, "Erreur syscall: %s\n", strerror(errno));
        return (false);
    }
    return (true);
}

bool    ft_parse_links(char links[500])
{
    // faire la requete et recuperer la page html
    const char *http = "http://";
    const char *https = "https://";

    if (!strncmp(http, links, 7))
        memmove(links, links + 7, strlen(links + 7));
    else if (!strncmp(https, links, 8))
        memmove(links, links + 8, strlen(links + 8));
    else
        return (false);
    return (true);        
}

bool    getHtmlPage(t_spider *data, int sfd)
{
    char    buff[4096];
    int     bytesRead = 0;

    while ((bytesRead = recv(sfd, buff, 4095, 0)) > 0)
    {
        buff[bytesRead] = 0;
        data->html_page = strjoin(data->html_page, buff, true);
        if (!data->html_page)   
            return (false);
    }
    if (!data->html_page)
        return (fprintf(stderr, "Error no request recv\n"), false);
    if (!get_links(data))
        return (false);
    if (!find_images(data))
        return (false);
    return (true);
}

bool    request_on_other_http_page(t_spider *data, struct addrinfo *result, struct addrinfo *rp)
{
    for (int i = 0; i < data->deepness && data->links_name_tab[i]; i++)
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
        // send
        if (!send_request(data, data->site_fd, data->links_name_tab[i]))
            return (close(data->site_fd), false);
        if (!getHtmlPage(data, data->site_fd))
            return (close(data->site_fd), false);
        free(data->html_page);
        data->html_page = NULL;
        close(data->site_fd);
    }
    return (true);
}

bool    scrapper(t_spider *data)
{
    int s = 0;
    struct addrinfo hints, *result = NULL, *rp = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    // -------
    if (data->https)
    {
        if (!https_request(data))
            return (false);
    }
    else
    {
        // get addr info
        s = getaddrinfo(data->hostname, "80", &hints, &result);
        if (s != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            return (false);
        }

        // connect
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
        if (rp == NULL)
            return (freeaddrinfo(result), err_msg("Could not connect to /\n"),
                 close(data->site_fd), false);
        // send
        if (!send_request(data, data->site_fd, "/"))
            return (freeaddrinfo(result), close(data->site_fd), false);
        if (!getHtmlPage(data, data->site_fd))
            return (freeaddrinfo(result), close(data->site_fd), false);
        free(data->html_page);
        data->html_page = NULL;
        close(data->site_fd);

        if (!request_on_other_http_page(data, result, rp))
            return (freeaddrinfo(result), false);
    }

    for (int i = 0; data->links_name_tab[i]; i++)
        free(data->links_name_tab[i]);
    free(data->links_name_tab);
    data->links_name_tab = NULL;

    if (data->recursive)
    {
        for (int i = 0; data->img_name_tab[i]; i++)
        {
            // open name in dir
            if (!ft_openfile_in_dir(data, i))
                return (freeaddrinfo(result), false);                   
            if (!request_to_get_image(data, rp, result, i))
                continue ;

            if (!ft_http_recursive_download(data, data->site_fd, 0))
                continue ;

            close(data->img_fd);
                // ft_recursive_download(data);            
        }
    }
    else
    {
        for (int i = 0; data->img_name_tab[i]; i++)
        {
            // open name in dir
            if (!ft_openfile_in_dir(data, i))
                return (freeaddrinfo(result), false);                   
            if (!request_to_get_image(data, rp, result, i))
                continue ;

            if (!ft_http_iterative_download(data, data->site_fd))
                continue ;

            close(data->img_fd);
                // ft_recursive_download(data);            
        }
    }
    freeaddrinfo(result);
    return (true);
}