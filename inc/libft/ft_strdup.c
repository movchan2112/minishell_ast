/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:36:19 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 17:44:33 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = ft_strlen(s);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i <= len)
	{
		dup[i] = s[i];
		i++;
	}
	return (dup);
}
