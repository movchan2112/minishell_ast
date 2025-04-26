#include "minishell.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

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

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

