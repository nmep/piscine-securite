#include "spider.h"

bool    http_get_addr_info(int *s,  struct addrinfo *hints,  struct addrinfo *result)
{
    *s = getaddrinfo("httpforever.com", "80", hints, &result);
    if (*s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(*s));
        return (false);
    }
    return (true);
}

bool    https_get_addr_info(t_spider *data, int *s,  struct addrinfo *hints,  struct addrinfo *result)
{
    *s = getaddrinfo(data->hostname, "443", hints, &result);
    if (*s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(*s));
        return (false);
    }
    return (true);
}

bool    connect_socket( struct addrinfo *rp,  struct addrinfo *result, int *sfd)
{
    for (*rp = *result; rp != NULL; rp = rp->ai_next) {
        *sfd = socket(rp->ai_family, rp->ai_socktype, 
        rp->ai_protocol);
        if (*sfd == -1)
            continue;
        if (connect(*sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;
        close(*sfd);
    }
    freeaddrinfo(result);
    if (rp == NULL) {  
        fprintf(stderr, "Could not connect\n");
        close(*sfd);
        return (false);
    }
    return (true);
}