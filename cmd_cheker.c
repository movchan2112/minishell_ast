#include "minishell.h"

int buildin_checker(char *cmd)
{
    if(!cmd)
        return 0;
    char *argc1[] = {"cd","echo","pwd","exit"};    // check len after in while loop
    int i = 0;
    while (i < 4) // <----------here
    {
        if(ft_strcmp(cmd, argc1[i]) == 0)
            return(1);
        i++;
    }
    return(0);
}