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
    printf("[%s]", data->html_page);
    return (true);
}

bool    init_socket(t_spider *data)
{
    int sfd = 0, s = 0;
    struct addrinfo hints, *result = NULL, *rp = NULL;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    // -------
    if (data->https)
    {
        // if (!https_get_addr_info(data, &s, &hints, result) || !connect_socket(rp, result, &sfd))
        //     return (false);
        // // faire sur du https
        if (!https_request(data))
            return (false);
    }
    else
    {
        // get addr info
        s = getaddrinfo("httpforever.com", "80", &hints, &result);
        if (s != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            return (false);
        }

        // connect

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            sfd = socket(rp->ai_family, rp->ai_socktype, 
            rp->ai_protocol);
            if (sfd == -1) {
                fprintf(stderr, "Socket error sfd = -1\n");
                continue;
            }
            if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
                break;
            close(sfd);
        }
        freeaddrinfo(result);
        if (rp == NULL) {
            fprintf(stderr, "Could not connect\n");
            close(sfd);
            return (false);
        }
        // send
        if (!send_request(data, sfd))
            return (close(sfd), false);
        if (!getHtmlPage(data, sfd))
            return (close(sfd), false);
    }
    close(sfd);
    // si -r est present faire en recursive sinon en iteratif
    
    // printf("recv = %s\n", buff);
    return (true);
}