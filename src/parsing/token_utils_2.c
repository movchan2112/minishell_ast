/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:45:47 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 20:46:42 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*read_q(char *line)
{
	char	*next_line;
	char	*combined;
	char	*tmp;

	combined = ft_strdup(line);
	if (!combined)
		return (NULL);
	while (ft_flag(combined, ft_strlen(combined)))
	{
		next_line = readline("quotes$ ");
		if (!next_line)
			break ;
		tmp = str_append(combined, next_line, 1);
		free(combined);
		free(next_line);
		combined = tmp;
		if (!combined)
			break ;
	}
	return (combined);
}


int	quotes_checker_len(char *line)
{
	int		i;
	char	quotes;
	int		len;

	i = 0;
	quotes = 0;
	len = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && ft_flag(line, i) == 0)
			quotes = line[i];
		else if (quotes == line[i] && ft_flag(line, i) != 0)
			quotes = 0;
		else
			len++;
		i++;
	}
	return (len);
}


void	fill_without_quotes(char *dup, char *line, size_t len)
{
	char	quotes;
	int		old;
	size_t	new;

	quotes = 0;
	old = 0;
	new = 0;
	while (line[old] && new < len)
	{
		if ((line[old] == '\'' || line[old] == '\"') && ft_flag(line, old) == 0)
			quotes = line[old];
		else if (quotes == line[old] && ft_flag(line, old) != 0)
			quotes = 0;
		else
		{
			dup[new] = line[old];
			new++;
		}
		old++;
	}
	dup[new] = '\0';
}

char	*strip_quotes(char *line)
{
	char	*dup;
	size_t	len;

	len = quotes_checker_len(line);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	fill_without_quotes(dup, line, len);
	return (dup);
}



void	label_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (ft_strcmp(tokens->value, "|") == 0)
			tokens->type = TOKEN_PIPE;
		else if (ft_strcmp(tokens->value, "<") == 0)
			tokens->type = TOKEN_REDIR_IN;
		else if (ft_strcmp(tokens->value, ">") == 0)
			tokens->type = TOKEN_REDIR_OUT;
		else if (ft_strcmp(tokens->value, ">>") == 0)
			tokens->type = TOKEN_REDIR_APPEND;
		else if (ft_strcmp(tokens->value, "<<") == 0)
			tokens->type = TOKEN_HEREDOC;
		else if (ft_strcmp(tokens->value, ";") == 0)
			tokens->type = TOKEN_SEMI;
		else
			tokens->type = TOKEN_WORD;
		tokens = tokens->next;
	}
}