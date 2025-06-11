/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   musor_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:45:22 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/28 13:51:21 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	init_args_builder(t_args_builder *builder, int word_count)
{
	builder->args_array = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!builder->args_array)
	{
		perror("minishell: malloc for args_array failed");
		builder->error_flag = 1;
		return ;
	}
	builder->cmd_name_val = NULL;
	builder->current_idx = 0;
	builder->error_flag = 0;
}

static void	add_arg_to_builder(t_args_builder *builder, const char *value)
{
	if (builder->error_flag)
		return ;
	builder->args_array[builder->current_idx] = ft_strdup(value);
	if (!builder->args_array[builder->current_idx])
	{
		perror("minishell: ft_strdup for arg failed");
		for (int k = 0; k < builder->current_idx; k++)
			free(builder->args_array[k]);
		free(builder->args_array);
		builder->args_array = NULL;
		builder->error_flag = 1;
		return ;
	}
	if (builder->current_idx == 0)
		builder->cmd_name_val = builder->args_array[0];
	builder->current_idx++;
}

static int	build_args_list(t_token *start, t_token *end,
				t_args_builder *bs, int max)
{
	t_token	*cur;

	cur = start;
	while (cur && cur != end && bs->current_idx < max)
	{
		if (cur->type == TOKEN_WORD)
		{
			add_arg_to_builder(bs, cur->value);
			if (bs->error_flag)
				return (1);
		}
		cur = cur->next;
	}
	return (0);
}

char	**populate_args_array(t_token *start, t_token *end,
			int word_count, t_args_builder *bs)
{
	init_args_builder(bs, word_count);
	if (bs->error_flag)
		return (NULL);
	if (build_args_list(start, end, bs, word_count))
		return (NULL);
	bs->args_array[bs->current_idx] = NULL;
	if (!bs->cmd_name_val && word_count > 0)
	{
		fprintf(stderr,
			"minishell: internal parser error - no command name from words\n");
		free_argv_array(bs->args_array);
		bs->args_array = NULL;
		return (NULL);
	}
	return (bs->args_array);
}

int	count_words_in_segment(t_token *start, t_token *end)
{
	t_token	*current;
	int		count;

	count = 0;
	current = start;
	while (current != NULL && current != end)
	{
		if (current->type == TOKEN_WORD)
			count++;
		current = current->next;
	}
	return (count);
}
