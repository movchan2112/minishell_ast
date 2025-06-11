/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_operators.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:00:45 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/20 17:54:15 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Handles two-character operators like "<<" or ">>".
 * @param s Pointer to the current parsing state.
 * @param list Pointer to the head of the token list.
 * @return 1 if a two-character operator was handled, 0 otherwise.
 */
int	handle_double_char_operator(t_parse_state *s, \
	t_token **list, t_shell *shell)
{
	char	current_char;
	char	next_char;
	char	op_buffer[3];

	current_char = s->line[s->i];
	next_char = s->line[s->i + 1];
	if ((current_char == '<' && next_char == '<') || \
		(current_char == '>' && next_char == '>'))
	{
		finalize_and_add_token(list, s, shell);
		op_buffer[0] = current_char;
		op_buffer[1] = next_char;
		op_buffer[2] = '\0';
		add_operator_as_token(list, op_buffer);
		s->i += 2;
		return (1);
	}
	return (0);
}

/**
 * @brief Handles single-character operators (|, <, >) or spaces.
 * @param s Pointer to the current parsing state.
 * @param list Pointer to the head of the token list.
 * @return 1 if a single-character operator or space was handled, 0 otherwise.
 */
int	handle_single_char_operator_or_space(t_parse_state *s, \
	t_token **list, t_shell *shell)
{
	char	current_char;
	char	op_buffer[2];

	current_char = s->line[s->i];
	if (current_char == ' ' || current_char == '|' || \
		current_char == '<' || current_char == '>')
	{
		finalize_and_add_token(list, s, shell);
		if (current_char != ' ')
		{
			op_buffer[0] = current_char;
			op_buffer[1] = '\0';
			add_operator_as_token(list, op_buffer);
		}
		s->i++;
		return (1);
	}
	return (0);
}

/**
 * @brief Checks for and handles operators or spaces at the current position.
 * This function now delegates to more specific handlers.
 * @param s Pointer to the current parsing state.
 * @param list Pointer to the head of the token list.
 * @return 1 if an operator/space was handled, 0 otherwise.
 */
int	handle_operator_or_space(t_parse_state *s, t_token **list, \
	t_shell *shell)
{
	if (s->line[s->i + 1] != '\0')
	{
		if (handle_double_char_operator(s, list, shell))
			return (1);
	}
	if (handle_single_char_operator_or_space(s, list, shell))
		return (1);
	return (0);
}

/**
 * @brief Processes a single character from the input line during parsing.
 * It determines if the character is part of an operator, a quoted segment,
 * or a regular token character, and updates the parsing state accordingly.
 * @param state Pointer to the current parsing state.
 * @param token_list Pointer to the head of the token list.
 */
void	process_line_character(t_parse_state *state, \
	t_token **token_list, t_shell *shell)
{
	int	is_char_quoted;

	is_char_quoted = (ft_flag(state->line, state->i) != 0);
	if (!is_char_quoted)
	{
		if (handle_operator_or_space(state, token_list, shell))
			return ;
	}
	if (!state->token_in_progress)
	{
		state->start_of_token = state->i;
		state->token_in_progress = 1;
	}
	state->i++;
}
