#include "../inc/minishell.h"

int	calculate_new_buffer_size(size_t *size, size_t required)
{
	if (*size == 0)
	{
		*size = required + 256;
		return (1);
	}
	while (required > *size)
	{
		if (*size > SIZE_MAX / 2)
			return (0);
		*size *= 2;
	}
	return (1);
}

int	ensure_buffer_capacity(t_buffer_info *buf_info,
				size_t additional_needed)
{
	char	*new_buffer;
	size_t	required_total;
	size_t	new_size;

	required_total = *(buf_info->current_len) + additional_needed + 1;
	if (required_total > *(buf_info->current_size))
	{
		new_size = *(buf_info->current_size);
		if (!calculate_new_buffer_size(&new_size, required_total))
			return (0);
		new_buffer = (char *)malloc(new_size);
		if (!new_buffer)
			return (0);
		if (*(buf_info->buffer) && *(buf_info->current_len) > 0)
			ft_memcpy(new_buffer, *(buf_info->buffer),
				*(buf_info->current_len));
		new_buffer[*(buf_info->current_len)] = '\0';
		free(*(buf_info->buffer));
		*(buf_info->buffer) = new_buffer;
		*(buf_info->current_size) = new_size;
	}
	return (1);
}

int	append_char_to_buffer(char c, t_buffer_info *buf_info)
{
	if (!ensure_buffer_capacity(buf_info, 1))
		return (0);
	(*(buf_info->buffer))[*(buf_info->current_len)] = c;
	(*(buf_info->current_len))++;
	(*(buf_info->buffer))[*(buf_info->current_len)] = '\0';
	return (1);
}

int	append_string_to_buffer(const char *str, t_buffer_info *buf_info)
{
	size_t	len;

	if (!str)
		return (1);
	len = strlen(str);
	if (len == 0)
		return (1);
	if (!ensure_buffer_capacity(buf_info, len))
		return (0);
	memcpy((*(buf_info->buffer)) + *(buf_info->current_len), str, len);
	*(buf_info->current_len) += len;
	(*(buf_info->buffer))[*(buf_info->current_len)] = '\0';
	return (1);
}

int	handle_dollar_question(int *line_idx_ptr, t_buffer_info *buf_info,
								int last_exit_status)
{
	char	status_str_buffer[12]; // Buffer for $? (e.g., "-2147483648\0")

	(*line_idx_ptr) += 2; // Consume '$' and '?'
	snprintf(status_str_buffer, sizeof(status_str_buffer), "%d", last_exit_status);
	if (!append_string_to_buffer(status_str_buffer, buf_info))
		return (1); // Memory error
	return (0);
}