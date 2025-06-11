/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:12:04 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/20 17:52:51 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	extract_and_append_env_var(const char *line, int *line_idx_ptr,
									t_buffer_info *buf_info)
{
	int		var_start_idx;
	int		var_len;
	char	var_name_buffer[256];
	char	*env_value;

	var_start_idx = *line_idx_ptr;
	while (line[*line_idx_ptr] && \
		(ft_isalnum(line[*line_idx_ptr]) || line[*line_idx_ptr] == '_'))
		(*line_idx_ptr)++;
	var_len = *line_idx_ptr - var_start_idx;
	if (var_len > 0 && var_len < 255)
	{
		strncpy(var_name_buffer, line + var_start_idx, var_len);
		var_name_buffer[var_len] = '\0';
		env_value = getenv(var_name_buffer);
		if (env_value)
		{
			if (!append_string_to_buffer(env_value, buf_info))
				return (1);
		}
	}
	return (0);
}


static int	process_env_variable(const char *line, int *line_idx_ptr, \
								t_buffer_info *buf_info, int last_exit_status)
{
	char    next_char;

	next_char = line[*line_idx_ptr + 1];
	if (next_char == '?')
		return (handle_dollar_question(line_idx_ptr, buf_info, \
			last_exit_status));
	else if (ft_isalnum(next_char) || next_char == '_')
	{
		(*line_idx_ptr)++;
		return (extract_and_append_env_var(line, line_idx_ptr, buf_info));
	}
	else
	{
		if (!append_char_to_buffer(line[*line_idx_ptr], buf_info))
			return (1);
		(*line_idx_ptr)++;
		return (0);
	}
}

static int	init_main_buffer_and_state(char **res_buf_ptr, size_t *cur_size_ptr,
							size_t *cur_len_ptr, t_buffer_info *buf_info)
{
	*cur_size_ptr = 256;
	*res_buf_ptr = (char *)malloc(*cur_size_ptr * sizeof(char));
	if (!*res_buf_ptr)
		return (0);
	ft_bzero(*res_buf_ptr, *cur_size_ptr);
	*cur_len_ptr = 0;
	buf_info->buffer = res_buf_ptr;
	buf_info->current_size = cur_size_ptr;
	buf_info->current_len = cur_len_ptr;
	return (1);
}

static int	process_line_for_expansion(const char *line,
	t_buffer_info *buf_info, int last_exit_status)
{
	int	i;
	int	process_status;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && (ft_flag(line, i) != 1))
		{
			process_status = process_env_variable(line, &i,
					buf_info, last_exit_status);
			if (process_status != 0)
				return (0); // Error occurred
		}
		else
		{
			if (!append_char_to_buffer(line[i], buf_info))
				return (0); // Error occurred
			i++;
		}
	}
	return (1); // Success
}

char	*expand_vars(const char *line, t_shell *p_shell)
{
	char			*result_buffer_ptr;
	size_t			current_buffer_size;
	size_t			current_buffer_len;
	t_buffer_info	buffer_management_info;
	int				actual_last_exit_status;

	if (!line || !p_shell)
		return (NULL);
	actual_last_exit_status = p_shell->exit_status;
	if (!init_main_buffer_and_state(&result_buffer_ptr, &current_buffer_size, \
		&current_buffer_len, &buffer_management_info))
		return (NULL);
	if (!process_line_for_expansion(line, &buffer_management_info, \
		actual_last_exit_status))
	{
		free(result_buffer_ptr);
		return (NULL);
	}
	return (result_buffer_ptr);
}
