/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:50:43 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 19:51:38 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_flag(const char *line, int pos)
{
	int	i;
	int	single;
	int	dbl;

	i = 0;
	single = 0;
	dbl = 0;
	while (i < pos && line[i])
	{
		if (line[i] == '\'' && dbl == 0)
			single = !single;
		else if (line[i] == '\"' && single == 0)
			dbl = !dbl;
		i++;
	}
	if (single)
		return (1);
	else if (dbl)
		return (2);
	return (0);
}
