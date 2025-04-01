#include "spider.h"

// static char    *add_hostname_to_http_request(t_spider *data)
// {
//     char *http_request_model = "GET / HTTP/1.1\r\n"
//     "Host: \r\n"
//     "Connection: close\r\n"
//     "\r\n";

//     char *request = malloc(sizeof(char) * (strlen(http_request_model) 
//                                         + strlen(data->hostname) + 1));
//     if (!request)
//     {
//         fprintf(stderr, "Error malloc, add hostname\n");
//         return (NULL);
//     }

//     int i = 0;
//     for (; i < 22; i++) 
//         request[i] = http_request_model[i];
//     http_request_model += 22;
//     request[i] = 0;
//     strcat(request, data->hostname);
//     strcat(request, http_request_model);
//     return (request);
// }

bool    send_request(t_spider *data, int sfd)
{
    // char *http_request = add_hostname_to_http_request(data);
    (void)data;
    char *http_request = "GET / HTTP/1.1\r\n"
    "Host: httpforever.com \r\n"
    "Connection: close\r\n"
    "\r\n";
    if (!http_request)
    {
        fprintf(stderr, "Error malloc, add hostname\n");
        return (NULL);
    }
    printf("request = %s\n", http_request);
    send(sfd, http_request, strlen(http_request), 0);
    // ft_free(http_request);
    return (true);
}

void    getHtmlPage(int sfd)
{
    char buff[4096];
    int bytesRead = 0;

    while ((bytesRead = recv(sfd, buff, 4095, 0)) > 0)
    {
        printf("j'ai lu %d\n", bytesRead);
        printf("[%s]", buff);
    }
    printf("sortie de boucle j'ai lu %d\n", bytesRead);

}

bool    init_socket(t_spider *data)
{
    int sfd, s;
    struct addrinfo hints, *result, *rp;
    
    memset(&hints, 0, sizeof(hints));
    
    s = getaddrinfo("httpforever.com", "80", &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return (false);
    }
    
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, 
            rp->ai_protocol);
            if (sfd == -1)
            continue;
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
        
    // -------
    send_request(data, sfd);
        
    // si -r est present faire en recursive sinon en iteratif
    getHtmlPage(sfd);
    
    // printf("recv = %s\n", buff);
    return (true);
}