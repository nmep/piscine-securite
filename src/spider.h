#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>

typedef struct s_spider
{
    bool recursive;

    char   *hostname;
    char   pathName[1024];
    int    deepness;
    bool   pathNameSelected;
    bool   deepnessSelected;


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


void    usage(void);
int     ft_strcmp(char *str1, char *str2);
int     ft_atoi (char *str);
void    ft_strcpy(char *src, char *dest);
bool    is_digit(char *str);
bool    is_dir(char *pathName);
void    print_data(t_spider data);
int     strstr_index(char *str, char *to_find);
void    ft_free(void *ptr);

/*  ############################################################# */
/* #                                                             #*/
/* #                                                             #*/
/* #                         SOCKET                              #*/
/* #                                                             #*/
/* #                                                             #*/
/*  ############################################################# */

bool    init_socket(t_spider *data);
bool    send_request(t_spider *data, int sfd);

