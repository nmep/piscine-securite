#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>

# include  "openssl/bio.h"
# include  "openssl/ssl.h"
# include  "openssl/err.h"

typedef struct s_spider
{
    bool   recursive;
    bool   https;

    int    deepness;
    int    site_fd;
    char   *hostname;
    char   pathName[1024];
    bool   pathNameSelected;
    bool   deepnessSelected;
    char   *html_page;
    char   **img_name_tab;
}   t_spider;

/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         PARSING                             #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */

bool    arg_pars(char **av, t_spider *data);


/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         TOOLs                               #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */

int     ft_strlen(char *str);
int     ft_strlen_2D(char **str);
void    usage(void);
int     ft_strcmp(char *str1, char *str2);
int     ft_atoi (char *str);
void    ft_strcpy(char *src, char *dest);
bool    is_digit(char *str);
bool    is_dir(char *pathName);
void    print_data(t_spider data);
int     strstr_index(char *str, char *to_find);
void    ft_free(void *ptr);
char    *strjoin(char *dest, char *src, bool free_dest);

/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         SOCKET                              #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */

bool    http_get_addr_info(t_spider *data, int *s,  struct addrinfo *hints,  struct addrinfo *result);
bool    https_get_addr_info(t_spider *data, int *s,  struct addrinfo *hints,  struct addrinfo *result);
bool    connect_socket(struct addrinfo *result, int *sfd);
bool    send_request(t_spider *data, int sfd);

/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         OPENSSL                             #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */

bool    https_request(t_spider *data);


/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         SCRAPPER                            #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */


bool    scrapper(t_spider *data);
bool    find_images(t_spider *data);
