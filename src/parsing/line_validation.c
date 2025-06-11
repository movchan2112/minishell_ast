/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:34:31 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 18:58:06 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Appends a string to the main result string.
 * Manages memory based on the assumption of how user's `str_append` works:
 * `new_main = str_append(old_main, add_str, 0)` \
 * means old_main is NOT freed by str_append.
 * This function handles freeing the old *main_str and updates it.
 *
 * @param main_str Pointer to the main result string (will be updated).
 * @param add_str String to append.
 * @param free_add If true, add_str is freed after appending.
 * @return 0 on success, 1 on failure (e.g., malloc error).
 */
static int	append_to_result_str(char **main_str, char *add_str, int free_add)
{
	char	*new_str_ptr;
	char	*old_main_ptr;

	if (!add_str)
		return (0);
	old_main_ptr = *main_str;
	new_str_ptr = str_append(old_main_ptr, add_str, 0);
	if (free_add)
		free(add_str);
	if (!new_str_ptr)
		return (1);
	free(old_main_ptr);
	*main_str = new_str_ptr;
	return (0);
}

/**
 * @brief Processes a quoted segment (single or double) from the input line.
 * Appends the literal content within quotes to the result string.
 *
 * @param l The input line.
 * @param i Pointer to the current index in the line (will be updated).
 * @param r_str Pointer to the result string.
 * @return 0 on success, 1 on failure.
 */

int	process_quoted_segment(const char *l, int *i, char **r_str)
{
	int		start;
	char	quote_char;
	char	*segment;

	quote_char = l[*i];
	(*i)++;
	start = *i;
	while (l[*i] && l[*i] != quote_char)
		(*i)++;
	segment = ft_strndup(l + start, *i - start);
	if (!segment)
		return (1);
	if (append_to_result_str(r_str, segment, 1))
		return (1);
	if (l[*i] == quote_char)
		(*i)++;
	return (0);
}
/**
 * @brief Processes the $? expansion for exit status.
 *
 * @param i Pointer to the current index in the line (will be updated).
 * @param r_str Pointer to the result string.
 * @param sh Pointer to the shell structure.
 * @return 0 on success, 1 on failure.
 */

int	process_exit_status_expansion(int *i, char **r_str, t_shell *sh)
{
	char	*exit_s;

	exit_s = ft_itoa(sh->exit_status);
	if (!exit_s)
		return (1);
	if (append_to_result_str(r_str, exit_s, 1))
		return (1);
	*i += 2;
	return (0);
}
/**
 * @brief Processes environment variable expansion (e.g., $VAR_NAME).
 *
 * @param l The input line.
 * @param i Pointer to the current index in the line (will be updated).
 * @param r_str Pointer to the result string.
 * @param sh Pointer to the shell structure.
 * @return 0 on success, 1 on failure.
 */

int	process_variable_expansion(const char *l, int *i, char **r_str, \
	t_shell *sh)
{
	int		key_start;
	char	*key;
	char	*val;

	(*i)++;
	key_start = *i;
	while (ft_isalnum(l[*i]) || l[*i] == '_')
		(*i)++;
	key = ft_strndup(l + key_start, *i - key_start);
	if (!key)
		return (1);
	val = get_env_value(sh->env, key);
	if (val)
	{
		if (append_to_result_str(r_str, val, 0))
		{
			free(key);
			return (1);
		}
	}
	free(key);
	return (0);
}
/**
 * @brief Appends a single regular (non-special) character to the result string.
 *
 * @param l The input line.
 * @param i Pointer to the current index in the line (will be updated).
 * @param r_str Pointer to the result string.
 * @return 0 on success, 1 on failure.
 */

int	process_regular_char(const char *l, int *i, char **r_str)
{
	char	temp_char_str[2];

	temp_char_str[0] = l[*i];
	temp_char_str[1] = '\0';
	if (append_to_result_str(r_str, temp_char_str, 0))
		return (1);
	(*i)++;
	return (0);
}


