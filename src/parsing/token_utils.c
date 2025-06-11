/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 20:32:36 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 20:33:35 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_special_symbol(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

int	is_two_char_operator(const char *line, int i)
{
	return ((line[i] == '<' && line[i + 1] == '<') || \
	(line[i] == '>' && line[i + 1] == '>'));
}
#include <stddef.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}
