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
	new->type = -1;
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

char *str_append(const char *s1, const char *s2, int add_newline)
{
	char *result;
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	size_t total = len1 + len2 + (add_newline ? 1 : 0) + 1;

	result = malloc(total);
	if (!result)
		return NULL;

	strcpy(result, s1);
	if (add_newline)
	{
		result[len1] = '\n';
		strcpy(result + len1 + 1, s2);
	}
	else
	{
		strcpy(result + len1, s2);
	}
	return result;
}

char *read_q(char *line)
{
	char *next_line;
	char *combined = strdup(line);  // start with a copy of the original
	if (!combined)
		return NULL;

	while (ft_flag(combined, strlen(combined)))
	{
		next_line = readline("quotes$ ");
		if (!next_line)
			break;

		char *tmp = str_append(combined, next_line, 0); // add newline
		free(combined);
		free(next_line);
		combined = tmp;
		if (!combined)
			break;
	}
	return combined;
}

char *ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_len;
	char	*sub;

	if (!s)
		return (NULL);

	s_len = 0;
	while (s[s_len])
		s_len++;

	if (start >= s_len)
		return (strdup(""));  // empty string if start is past end

	if (len > s_len - start)
		len = s_len - start;

	sub = malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);

	i = 0;
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char *strip_quotes(const char *s)
{
	size_t len = ft_strlen(s);

	if (len >= 2 && 
	   ((s[0] == '"' && s[len - 1] == '"') ||
		(s[0] == '\'' && s[len - 1] == '\'')))
	{
		return ft_substr(s, 1, len - 2); // remove both ends
	}
	return ft_strdup(s); // return as is
}



t_token *parse_line(char *line)
{
	int i = 0;
	int start = 0;
	int in_token = 0;
	t_token *token_list = NULL;
	if(ft_flag(line, strlen(line)))
		line = read_q(line);

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
	return token_list;
}
