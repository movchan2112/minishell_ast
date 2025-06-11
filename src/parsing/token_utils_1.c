/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:12:04 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/28 14:02:45 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	add_token_back(t_token **head, t_token *new)
{
	t_token	*current;

	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}


void	finalize_and_add_token(t_token **list, t_parse_state *s, t_shell *shell)
{
	char	*raw_val;
	char	*expanded_val;
	char	*final_val;

	if (!s->token_in_progress || s->start_of_token >= s->i)
		return ;
	raw_val = ft_strndup(s->line + s->start_of_token, \
							s->i - s->start_of_token);
	if (!raw_val)
		return ;
	expanded_val = expand_vars(raw_val, shell);
	free(raw_val);
	if (!expanded_val)
		return ;
	final_val = strip_quotes(expanded_val);
	free(expanded_val);
	if (!final_val)
		return ;
	add_token_back(list, init_token(final_val));
	free(final_val);
	s->token_in_progress = 0;
}

void	add_operator_as_token(t_token **list, const char *op_str)
{
	char	*op_val_dup;

	op_val_dup = ft_strdup(op_str);
	if (!op_val_dup)
		return ;
	add_token_back(list, init_token(op_val_dup));
	free(op_val_dup);
}

void delete_empty_value_nodes(t_token **head)
{
	t_token *current = *head;
	t_token *prev = NULL;

	while (current != NULL)
	{
		if (current->value && current->value[0] == '\0') // empty string check
		{
			t_token *to_delete = current;

			if (prev == NULL) // Deleting head node
				*head = current->next;
			else
				prev->next = current->next;

			current = current->next;
			free(to_delete->value);
			free(to_delete);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

t_token	*parse_line(const char *input_line, t_shell *shell)
{
	t_parse_state	state;
	t_token			*token_list;

	if (!input_line)
		return (NULL);
	token_list = NULL;
	state.line = input_line;
	state.i = 0;
	state.start_of_token = 0;
	state.token_in_progress = 0;
	while (state.line[state.i])
		process_line_character(&state, &token_list, shell);
	finalize_and_add_token(&token_list, &state, shell);
	delete_empty_value_nodes(&token_list);
	
	return (token_list);
}


