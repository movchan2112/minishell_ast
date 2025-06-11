#include "minishell.h"

char	*my_strtok(char *str, const char *divider)
{
	static char	*saved_str;
	char		*token_start;

	if (str)
		saved_str = str;
	if (!saved_str)
		return (NULL);
	while (*saved_str && ft_strchr(divider, *saved_str))
		saved_str++;
	if (!*saved_str)
		return (NULL);
	token_start = saved_str;
	while (*saved_str && !ft_strchr(divider, *saved_str))
		saved_str++;
	if (*saved_str)
	{
		*saved_str = '\0';
		saved_str++;
	}
	else
		saved_str = NULL;
	return (token_start);
}



