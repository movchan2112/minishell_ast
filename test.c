#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

int	ft_flag(const char *line, int pos)
{
    int i = 0;
    int single = 0;
    int dbl = 0;

    while (i < pos && line[i])
    {
        if (line[i] == '\'' && dbl == 0)
            single = !single;
        else if (line[i] == '\"' && single == 0)
            dbl = !dbl;
        i++;
    }

    if (single)
        return (1);
    else if (dbl)
        return (2);
    return (0);
}


int quotes_checker(char *line)
{
    int i = 0;
    char quotes = 0;
    int len = 0;

    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '\"') && ft_flag(line, i) == 0)
        {
            // printf("%c its first in pos - %d\n",line[i],i); ---delete
            quotes = line[i];
        }
        else if(quotes == line[i] && ft_flag(line,i) != 0)
        {
            // printf("%c sedond in pos - %d\n",line[i],i); ---delete
            quotes = 0;
        }
        else
        {
            len++;
            printf("%c",line[i]); //---delete
        }
        i++;
    }
    return(len);
}

int main(int ac, char **av)
{
    char *line = NULL;
    line = readline("test$: ");
    printf("%d for ft_flag\n", ft_flag(line,2));
    printf("%d", quotes_checker(line));
    
    return 0;
}