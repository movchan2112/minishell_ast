/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 19:58:48 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/09 19:59:05 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_env_list(t_env *env)
{
	while (env)
	{
		if (env->val)
			printf("%s=%s\n", env->key, env->val);
		env = env->next;
	}
}
