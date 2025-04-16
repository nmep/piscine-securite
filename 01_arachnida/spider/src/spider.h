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
#include <fcntl.h>  

# include  "openssl/bio.h"
# include  "openssl/ssl.h"
# include  "openssl/err.h"

typedef struct s_spider
{
    bool   recursive;
    bool   https;

    int    deepness;
    int    site_fd;
    int    img_fd;
    char   *hostname;
    char   pathName[1024];
    bool   pathNameSelected;
    bool   deepnessSelected;
    char   *html_page;
    char   *img;
    char   **img_name_tab;
    char   **links_name_tab;
}   t_spider;

/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         PARSING                             #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */

bool    arg_pars(char **av, t_spider *data);
bool    Url_to_Hostname(t_spider *data, char *url);

/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         TOOLs                               #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */

void    usage(void);
void    print_data(t_spider data);
bool    err_msg(const char *err_msg);

int     ft_strlen(char *str);
int     ft_strlen_2D(char **str);
int     ft_strcmp(char *str1, char *str2);
void    ft_strcpy(char *src, char *dest);
void    ft_strncpy(char *src, char *dest, int n);

int     ft_atoi (char *str);

bool    is_digit(char *str);
bool    is_dir(char *pathName);
int     strstr_index(char *str, char *to_find);
bool    checkDouble(char **links_name_tab, char *new_link);

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
bool    send_request(t_spider *data, int sfd, char *path);

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
bool    ft_parse_links(char links[500]);
bool    get_links(t_spider *data);
bool    find_images(t_spider *data);

bool    ft_recursive_download(t_spider *data);
bool    ft_iterative_download(t_spider *data, int sfd);

bool    request_to_get_image(t_spider *data, struct addrinfo *rp, struct addrinfo *result, int i);
bool    ft_openfile_in_dir(t_spider *data, int i);


