#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef struct s_spider
{
    bool recursive; // demand ? oui non

    char   pathName[1024];
    bool   pathNameSelected;
    int    deepness;
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
int     atoi (char *str);
void    ft_strcpy(char *src, char *dest);
bool    is_digit(char *str);
bool    is_dir(char *pathName);
