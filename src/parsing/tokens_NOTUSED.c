#include "minishell.h"

void print_cmd(t_cmd *cmd)
{
	if (!cmd)
	{
		printf("(null cmd)\n");
		return;
	}
	printf("Command: %s\n", cmd->cmd);
	printf("Args: ");
	int i = 0;
	while (cmd->args && cmd->args[i])
	{
		printf("[%s] ", cmd->args[i]);
		i++;
	}
	printf("\n");

	if (cmd->outfile)
		printf("Outfile: %s (append=%d)\n", cmd->outfile, cmd->append);

	if (cmd->infile)
		printf("Infile: %s\n", cmd->infile);

	if (cmd->heredoc)
		printf("Heredoc: %s\n", cmd->heredoc);
}

void	print_ast(t_ast *node, int level)
{
	if (!node)
	{
		for (int i = 0; i < level; i++)
			printf("    ");
		printf("[NULL]\n");
		return;
	}

	for (int i = 0; i < level; i++)
		printf("    ");

	switch (node->type)
	{
		case NODE_PIPE:
			printf("[PIPE]\n");
			break;
		case NODE_REDIR_OUT:
			printf("[REDIR_OUT]\n");
			break;
		case NODE_REDIR_APPEND:
			printf("[REDIR_APPEND]\n");
			break;
		case NODE_REDIR_IN:
			printf("[REDIR_IN]\n");
			break;
		case NODE_HEREDOC:
			printf("[HEREDOC]\n");
			break;
		case NODE_CMD:
			printf("[CMD]\n");
			for (int i = 0; i < level + 1; i++)
				printf("    ");
			print_cmd(node->cmd);  // ваша функция, которая печатает команду
			break;
		default:
			printf("[UNKNOWN NODE TYPE: %d]\n", node->type);
			break;
	}

	print_ast(node->left, level + 1);
	print_ast(node->right, level + 1);
}
void	label_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (ft_strcmp(tokens->value, "|") == 0)
			tokens->type = TOKEN_PIPE;
		else if (ft_strcmp(tokens->value, "<") == 0)
			tokens->type = TOKEN_REDIR_IN;
		else if (ft_strcmp(tokens->value, ">") == 0)
			tokens->type = TOKEN_REDIR_OUT;
		else if (ft_strcmp(tokens->value, ">>") == 0)
			tokens->type = TOKEN_REDIR_APPEND;
		else if (ft_strcmp(tokens->value, "<<") == 0)
			tokens->type = TOKEN_HEREDOC;
		else if (ft_strcmp(tokens->value, ";") == 0)
			tokens->type = TOKEN_SEMI;
		else
			tokens->type = TOKEN_WORD;
		tokens = tokens->next;
	}
}



t_token	*init_token(const char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = -1;
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->next = (NULL);
	return (new);
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

// char *read_q(char *line)
// {
// 	char *next_line;
// 	char *combined = strdup(line);  // start with a copy of the original
// 	if (!combined)
// 		return NULL;

// 	while (ft_flag(combined, strlen(combined)))
// 	{
// 		next_line = readline("quotes$ ");
// 		if (!next_line)
// 			break;

// 		char *tmp = str_append(combined, next_line, 1); // add newline
// 		free(combined);
// 		free(next_line);
// 		combined = tmp;
// 		if (!combined)
// 			break;
// 	}
// 	return combined;
// }

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
	size_t new = 0;

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
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}


char *trim_spaces(char *str)
{
    int start = 0;
    int end = ft_strlen(str) - 1;

    while (str[start] && ft_isspace(str[start]))
        start++;
    while (end >= start && ft_isspace(str[end]))
        end--;

    return ft_strndup(str + start, end - start + 1);
}


// int check_and_add_token(t_token **token_list, char *line, int start, int i)
// {
//     char *clean_token;
//     char *raw_token = ft_strndup(line + start, i - start);
//     char *expanded_token = expand_vars(raw_token);
//     if(expanded_token)
//         clean_token = strip_quotes(expanded_token);
//     if (clean_token)
//         add_token_back(token_list, init_token(clean_token));
//     if(expanded_token)
//         free(expanded_token);
//     free(raw_token);
//     return 0; // this will change in_token to 0
// }

void	ft_fprintf_stderr(const char *prefix, const char *middle, const char *suffix)
{
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(middle, 2);
	ft_putstr_fd(suffix, 2);
}