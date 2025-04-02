#include "spider.h"

static char    *add_hostname_to_request(t_spider *data)
{
    char *http_request_model = "GET / HTTP/1.1\r\n"
    "Host: \r\n"
    "Connection: close\r\n"
    "\r\n";

    char *request = malloc(sizeof(char) * (strlen(http_request_model) 
                                        + strlen(data->hostname) + 1));
    if (!request)
    {
        fprintf(stderr, "Error malloc, add hostname\n");
        return (NULL);
    }

    int i = 0;
    for (; i < 22; i++) 
        request[i] = http_request_model[i];
    http_request_model += 22;
    request[i] = 0;
    strcat(request, data->hostname);
    strcat(request, http_request_model);
    return (request);
}

bool    send_request(t_spider *data, int sfd)
{
    char *request = add_hostname_to_request(data);
    if (!request)
    {
        fprintf(stderr, "Error malloc, add hostname\n");
        return (NULL);
    }
    send(sfd, request, strlen(request), 0);
    ft_free(request);
    return (true);
}

void    getHtmlPage(int sfd)
{
    char buff[4096];
    int bytesRead = 0;

    while ((bytesRead = recv(sfd, buff, 4095, 0)) > 0)
    {
        // printf("j'ai lu %d\n", bytesRead);
        // printf("[%s]", buff);
    }
}

bool    init_socket(t_spider *data)
{
    int sfd = 0, s = 0;
    struct addrinfo hints, *result = NULL, *rp = NULL;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
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
        if (http_get_addr_info(&s, &hints, result) || connect_socket(rp, result, &sfd) 
                || send_request(data, sfd))
            return (false);
        getHtmlPage(sfd);
        // faire sur du http
    }

    // si -r est present faire en recursive sinon en iteratif
    
    // printf("recv = %s\n", buff);
    return (true);
}