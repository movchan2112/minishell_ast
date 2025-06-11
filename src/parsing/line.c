/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:23:48 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 18:56:25 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Preprocesses a line for variable expansion and quote handling.
 * Adheres to Norminette rules by using helper functions.
 *
 * @param line The input line string.
 * @param shell Pointer to the shell structure.
 * @return A new string with expansions, or NULL on allocation failure.
 * The caller is responsible for freeing the returned string.
 */

char	*preprocess_line(const char *line, t_shell *shell)
{
	char	*result_str;
	int		i;
	int		status;

	result_str = ft_strdup("");
	if (!result_str)
		return (NULL);
	i = 0;
	status = 0;
	while (line[i] && status == 0)
	{
		if (line[i] == '\'' || line[i] == '"')
			status = process_quoted_segment(line, &i, &result_str);
		else if (line[i] == '$' && line[i + 1] == '?')
			status = process_exit_status_expansion(&i, &result_str, shell);
		else if (line[i] == '$' && \
				(ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
			status = process_variable_expansion(line, &i, &result_str, shell);
		else
			status = process_regular_char(line, &i, &result_str);
	}
	if (status != 0)
		return (free(result_str), NULL);
	return (result_str);
}
