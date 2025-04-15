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
    printf("html page = %s\n", data->html_page);   
    if (!get_links(data))
        return (false);
    if (!find_images(data))
        return (false);
    return (true);
}

bool    request_on_other_http_page(t_spider *data)
{
    for (int i = 0; i < data->deepness && data->links_name_tab[i]; i++)
    {
        printf("test request on %s\n", data->links_name_tab[i]);
        if (!send_request(data, data->site_fd, data->links_name_tab[i]))
            continue;
        printf("succed\n");
        if (!getHtmlPage(data, data->site_fd))
            return (close(data->site_fd), false);
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
        printf("https\n");
        if (!https_request(data))
            return (false);
    }
    else
    {
        // get addr info
        printf("http\n");
        printf("hostname = %s\n", data->hostname);
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
        freeaddrinfo(result);
        if (rp == NULL) {
            fprintf(stderr, "Could not connect\n");
            close(data->site_fd);
            return (false);
        }
        // send
        if (!send_request(data, data->site_fd, "/"))
            return (close(data->site_fd), false);
        if (!getHtmlPage(data, data->site_fd))
            return (close(data->site_fd), false);
        free(data->html_page);
        data->html_page = NULL;

        request_on_other_http_page(data);
        if (data->links_name_tab)
        {
            for (int i = 0; data->links_name_tab[i]; i++) {
                printf("links name tab [%d] = %s\n", i, data->links_name_tab[i]);
            }
        }
    }
    
    // si -r est present faire en recursive sinon en iteratif


    return (true);
}