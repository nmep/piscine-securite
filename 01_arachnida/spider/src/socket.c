#include "spider.h"

bool    send_request(t_spider *data, int sfd)
{
    char request[1024];
    if (snprintf(request, sizeof(request), 
            "GET / HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "\r\n", data->hostname) <= 0)
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

bool    getHtmlPage(t_spider *data, int sfd)
{
    char buff[4096];
    int bytesRead = 0;

    while ((bytesRead = recv(sfd, buff, 4095, 0)) > 0)
    {
        buff[bytesRead] = 0;
        data->html_page = strjoin(data->html_page, buff, true);
        if (!data->html_page)
            return (false);
    }
    // find_images(data);
    printf("[%s]\n", data->html_page);
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
        printf("http\n");
        // get addr info
        s = getaddrinfo("httpforever.com", "80", &hints, &result);
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
        if (!send_request(data, data->site_fd))
            return (close(data->site_fd), false);
        if (!getHtmlPage(data, data->site_fd))
            return (close(data->site_fd), false);
    }
    
    // si -r est present faire en recursive sinon en iteratif


    // printf("recv = %s\n", buff);
    return (true);
}