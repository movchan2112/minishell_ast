/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:19:51 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 19:00:15 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
 * @brief Frees resources associated with a single command execution.
 * @param shell The shell structure.
 */

void	free_command_resources(t_shell *shell)
{
	if (shell->tokens)
	{
		free_token_list(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
}
