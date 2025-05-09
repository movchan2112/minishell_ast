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

		char *tmp = str_append(combined, next_line, 1); // add newline
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


/// -------------------------------------------------------------------------------------------------------------- ///
/// ------------------------------------------------quotes-------------------------------------------------------- ///
/// -------------------------------------------------------------------------------------------------------------- ///


int quotes_checker_len(char *line)
{
    int i = 0;
    char quotes = 0;
    int len = 0;

    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '\"') && ft_flag(line, i) == 0)
        {
            // printf("%c its first in pos - %d\n",line[i],i); ---delete
            quotes = line[i];
        }
        else if(quotes == line[i] && ft_flag(line,i) != 0)
        {
            // printf("%c sedond in pos - %d\n",line[i],i); ---delete
            quotes = 0;
        }
        else
        {
            len++;
            // printf("%c",line[i]); //---delete
			// write(1, &line[i], 1);
        }
        i++;
    }
    return(len);
}

char *strip_quotes(char *line)
{
	char quotes = 0;
	size_t l = quotes_checker_len(line);
    int old = 0;
	int new = 0;

    char *dup = malloc(l + 1);
    if (!dup)
        return NULL;
    while (line[old] && new < l)
    {
        if ((line[old] == '\'' || line[old] == '\"') && ft_flag(line, old) == 0)
            quotes = line[old];
        else if(quotes == line[old] && ft_flag(line,old) != 0)
            quotes = 0;
        else
        {

			dup[new] = line[old];
            new++;
        }
        old++;
    }
    dup[new] = '\0';
    return dup;
}


/// ------------------------------------------------VARIABLES----------------------------------------------------- ///
/// ------------------------------------------------VARIABLES----------------------------------------------------- ///
/// ------------------------------------------------VARIABLES----------------------------------------------------- ///


char *expand_vars(const char *line)
{
    size_t result_size = 4096;
    char *result = calloc(result_size, 1);
    if (!result)
        return NULL;

    size_t res_i = 0;

    for (int i = 0; line[i];)
    {
        if (line[i] == '$' && ft_flag(line, i) != 1) // не в одинарных
        {
            i++; // skip $

            int start = i;
            while (isalnum(line[i]) || line[i] == '_')
                i++;

            int len = i - start;

            if (len > 0)
            {
                char var[256] = {0};
                strncpy(var, line + start, len);

                char *value = getenv(var);
                if (value)
                {
                    size_t val_len = strlen(value);
                    if (res_i + val_len >= result_size - 1)
                    {
                        result_size *= 2;
                        result = realloc(result, result_size);
                        if (!result)
                            return NULL;
                    }
                    strcpy(result + res_i, value);
                    res_i += val_len;
                }
            }
            // else: `$` with no variable — insert nothing (like bash)
        }
        else
        {
            result[res_i++] = line[i++];
        }
    }

    result[res_i] = '\0';
    return result;
}


int check_and_add_token(t_token **token_list, char *line, int start, int i)
{
    char *clean_token;
    char *raw_token = ft_strndup(line + start, i - start);
    char *expanded_token = expand_vars(raw_token);
    if(expanded_token)
        clean_token = strip_quotes(expanded_token);
    if (clean_token)
        add_token_back(token_list, init_token(clean_token));
    if(expanded_token)
        free(expanded_token);
    free(raw_token);
    return 0; // this will change in_token to 0
}


t_token *parse_line(char *line)
{
    int i = 0;
    int start = 0;
    int in_token = 0;
    t_token *token_list = NULL;

    while (line[i])
    {
        int in_quote = ft_flag(line, i) != 0;

        // only treat these outside of quotes
        if (!in_quote)
        {
            // handle two-char redirs: << or >>
            if ((line[i] == '<' && line[i+1] == '<') ||
                (line[i] == '>' && line[i+1] == '>'))
            {
                if (in_token)
                    in_token = check_and_add_token(&token_list, line, start, i);

                char *op = ft_strndup(line + i, 2);
                add_token_back(&token_list, init_token(op));
                free(op);

                i += 2;
                continue;
            }
            // single-char separators: space, pipe, < or >
            else if (line[i] == ' ' || line[i] == '|' ||
                     line[i] == '<' || line[i] == '>')
            {
                // end of a normal word-token?
                if (in_token)
                    in_token = check_and_add_token(&token_list, line, start, i);

                // if it's a pipe or redir, add it as its own token
                if (line[i] == '|' || line[i] == '<' || line[i] == '>')
                {
                    char op[2] = { line[i], '\0' };
                    add_token_back(&token_list, init_token(ft_strdup(op)));
                }

                // reset and advance past the separator
                in_token = 0;
                i++;
                continue;
            }
        }

        // if here, we're either inside quotes, or a normal character
        if (!in_token)
        {
            // start a new token
            in_token = 1;
            start = i;
        }
        i++;
    }

    // flush the final token
    if (in_token)
        check_and_add_token(&token_list, line, start, i);

    return token_list;
}
