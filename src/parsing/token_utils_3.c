#include "../inc/minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

t_token	*init_token(const char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = -1;
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->next = (NULL);
	return (new);
}


/**
 * @brief Copies s1 and s2 into the destination string.
 * Optionally adds a newline character between s1 and s2.
 *
 * @param dest The destination buffer where strings will be copied.
 * @param params Pointer to a structure containing s1, s2, len1, and add_newline.
 */
static void	perform_string_copy(char *dest, t_str_copy_params *params)
{
	ft_strcpy(dest, params->s1);
	if (params->add_newline)
	{
		dest[params->len1] = '\n';
		ft_strcpy(dest + params->len1 + 1, params->s2);
	}
	else
		ft_strcpy(dest + params->len1, params->s2);
}

/**
 * @brief Concatenates two strings, s1 and s2, into a new string.
 * Optionally adds a newline character between s1 and s2.
 * The caller is responsible for freeing the returned string.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @param add_newline If non-zero, a newline char is added between s1 and s2.
 * @return A pointer to the newly allocated concatenated string,
 * or NULL if memory allocation fails or input strings are NULL.
 */
char	*str_append(const char *s1, const char *s2, int add_newline)
{
	char				*result;
	size_t				len1_val;
	size_t				len2;
	size_t				total_len;
	t_str_copy_params	copy_params;

	if (!s1 || !s2)
		return (NULL);
	len1_val = ft_strlen(s1);
	len2 = ft_strlen(s2);
	total_len = len1_val + len2 + 1;
	if (add_newline)
		total_len += 1;
	result = (char *)malloc(total_len);
	if (!result)
		return (NULL);
	copy_params.s1 = s1;
	copy_params.s2 = s2;
	copy_params.len1 = len1_val;
	copy_params.add_newline = add_newline;
	perform_string_copy(result, &copy_params);
	return (result);
}