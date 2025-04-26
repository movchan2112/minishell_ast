#include "minishell.h"

int	ft_flag(const char *line, int pos)
{
	int i = 0;
	int single = 0;
	int dbl = 0;

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


void print_env_list(t_env *env)
{
    while (env)
    {
        if (env->val)
            printf("%s=%s\n", env->key, env->val);
        env = env->next;
    }
}

t_token *init_token(const char *value)
{
	t_token *new = malloc(sizeof(t_token));
	if (!new)
		return NULL;
	new->value = strdup(value);
	new->type = NULL;
	if (!new->value)
	{
		free(new);
		return NULL;
	}
	new->next = NULL;
	return new;
}

void add_token_back(t_token **head, t_token *new)
{
	if (!*head)
	{
		*head = new;
		return;
	}
	t_token *current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}

int is_special_symbol(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

int is_two_char_operator(const char *line, int i)
{
	return ((line[i] == '<' && line[i + 1] == '<') ||
			(line[i] == '>' && line[i + 1] == '>'));
}
t_token *parse_line(char *line)
{
	int i = 0;
	int start = 0;
	int in_token = 0;
	t_token *token_list = NULL;

	while (line[i])
	{
		if (ft_flag(line, i) == 0) // only care about special stuff outside quotes
		{
			// handle two-char operators like << >>
			if (is_two_char_operator(line, i))
			{
				if (in_token)
				{
					char *new_token = ft_strndup(line + start, i - start);
					if (new_token)
						add_token_back(&token_list, init_token(new_token));
					free(new_token);
					in_token = 0;
				}
				char *op = ft_strndup(line + i, 2);
				add_token_back(&token_list, init_token(op));
				free(op);
				i += 2;
				continue;
			}
			// single-character special symbols
			else if (is_special_symbol(line[i]))
			{
				if (in_token)
				{
					char *new_token = ft_strndup(line + start, i - start);
					if (new_token)
						add_token_back(&token_list, init_token(new_token));
					free(new_token);
					in_token = 0;
				}
				char *op = ft_strndup(line + i, 1);
				add_token_back(&token_list, init_token(op));
				free(op);
				i++;
				continue;
			}
			// space ends token
			else if (line[i] == ' ' && in_token)
			{
				char *new_token = ft_strndup(line + start, i - start);
				if (new_token)
					add_token_back(&token_list, init_token(new_token));
				free(new_token);
				in_token = 0;
			}
			else if (line[i] != ' ' && in_token == 0)
			{
				in_token = 1;
				start = i;
			}
		}
		else if (in_token == 0 && line[i] != ' ')
		{
			// inside quotes: new token
			in_token = 1;
			start = i;
		}
		i++;
	}

	// Final token at end of line
	if (in_token)
	{
		char *new_token = ft_strndup(line + start, i - start);
		if (new_token)
			add_token_back(&token_list, init_token(new_token));
		free(new_token);
	}

	// // Debug output
	// t_token *tmp = token_list;
	// while (tmp)
	// {
	// 	printf("TOKEN: [%s]\n", tmp->value);
	// 	tmp = tmp->next;
	// }

	return token_list;
}
