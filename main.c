#include "minishell.h"

t_ast *create_ast_node(t_node_type type)
{
	t_ast *node = malloc(sizeof(t_ast));
	if (!node)
		return NULL;
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	return node;
}

void label_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (strcmp(tokens->value, "|") == 0)
			tokens->type = TOKEN_PIPE;
		else if (strcmp(tokens->value, "<") == 0)
			tokens->type = TOKEN_REDIR_IN;
		else if (strcmp(tokens->value, ">") == 0)
			tokens->type = TOKEN_REDIR_OUT;
		else if (strcmp(tokens->value, ">>") == 0)
			tokens->type = TOKEN_REDIR_APPEND;
		else if (strcmp(tokens->value, "<<") == 0)
			tokens->type = TOKEN_HEREDOC;
		else if (strcmp(tokens->value, ";") == 0)
			tokens->type = TOKEN_SEMI;
		else
			tokens->type = TOKEN_WORD;
		tokens = tokens->next;
	}
}
void print_tokens(t_token *tokens)
{
	char *token_type_str[] = {
		"TOKEN_PIPE",
		"TOKEN_REDIR_IN",
		"TOKEN_REDIR_OUT",
		"TOKEN_REDIR_APPEND",
		"TOKEN_HEREDOC",
		"TOKEN_WORD",
		"TOKEN_SEMI"
	};

	printf("Tokens list:\n");
	while (tokens)
	{
		if (tokens->type >= 0 && tokens->type <= TOKEN_SEMI)
			printf("Value: %-10s | Type: %s\n", tokens->value, token_type_str[tokens->type]);
		else
			printf("Value: %-10s | Type: UNKNOWN\n", tokens->value);
		tokens = tokens->next;
	}
}

t_ast *create_cmd_node(char *cmd, char **args)
{
	t_ast *node = create_ast_node(NODE_CMD);
	node->cmd = malloc(sizeof(t_cmd));
	node->cmd->cmd = cmd;
	node->cmd->args = args;
	node->cmd->outfile = NULL;
	node->cmd->append = 0;
	node->cmd->infile = NULL;
	node->cmd->heredoc = NULL;
	return node;
}

t_ast *build_ast_from_tokens(t_token *start, t_token *end)
{
	t_token *current = start;

	// 1. Найти пайп
	while (current && current != end)
	{
		if (current->type == TOKEN_PIPE)
		{
			t_ast *pipe_node = create_ast_node(NODE_PIPE);
			pipe_node->left = build_ast_from_tokens(start, current);       // всё до пайпа
			pipe_node->right = build_ast_from_tokens(current->next, end);   // всё после пайпа
			return pipe_node;
		}
		current = current->next;
	}

	// 2. Найти редиректы
	current = start;
	while (current && current != end)
	{
		if (current->type == TOKEN_REDIR_OUT)
		{
			t_ast *redir_node = create_ast_node(NODE_REDIR_OUT);
			redir_node->left = build_ast_from_tokens(start, current); // команда слева
			// redir_node->right будет NULL — файл запишем отдельно в структуру команды
			if (current->next)
			{
				redir_node->cmd = malloc(sizeof(t_cmd));
				redir_node->cmd->outfile = ft_strdup(current->next->value);; // имя файла
				redir_node->cmd->append = 0;
				redir_node->cmd->cmd = NULL;
				redir_node->cmd->args = NULL;
				redir_node->cmd->infile = NULL;
				redir_node->cmd->heredoc = NULL;
			}
			return redir_node;
		}
		current = current->next;
	}

	// 3. Иначе это обычная команда
	if (start)
	{
		// Собрать args
		int count = 0;
		current = start;
		while (current && current != end)
		{
			if (current->type == TOKEN_WORD)
				count++;
			current = current->next;
		}

		char **args = malloc(sizeof(char *) * (count + 1));
		current = start;
		int i = 0;
		while (current && current != end)
		{
			if (current->type == TOKEN_WORD)
				args[i++] = ft_strdup(current->value);
			current = current->next;
		}
		args[i] = NULL;

		return create_cmd_node(ft_strdup(args[0]), args);
	}

	return NULL;
}

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
void print_ast(t_ast *node, int level)
{
	if (!node)
		return;

	for (int i = 0; i < level; i++)
		printf("    ");

	if (node->type == NODE_PIPE)
		printf("[PIPE]\n");
	else if (node->type == NODE_REDIR_OUT)
		printf("[REDIR_OUT]\n");
	else if (node->type == NODE_CMD)
	{
		printf("[CMD]\n");
		for (int i = 0; i < level + 1; i++)
			printf("    ");
		print_cmd(node->cmd);
	}

	// Рекурсивно печатаем левое и правое поддеревья
	print_ast(node->left, level + 1);
	print_ast(node->right, level + 1);
}

void	ft_putstr(const char *s)
{
	if (!s)
		return;
	while (*s)
		write(1, s++, 1);
}


int exec_tree(t_shell *shell,t_ast *ast)
{
	int pid = 0;
	// if(tree->type == NODE_PIPE)
	// {
	// 	pid = fork();
	// 	exec_tree(tree->left);
	// 	exec_tree(tree->right);
	// }
	if (ast->type == NODE_REDIR_OUT)
	{
		int fd = open(ast->cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("minishell: open");
			// shell->exit_status = 1;
			return 1;
		}

		int saved_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);

		// ✅ ИСПОЛНЯЕМ ТОЛЬКО ЛЕВЫЙ УЗЕЛ, ГДЕ НАСТОЯЩАЯ КОМАНДА
		exec_tree(shell, ast->left);

		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		return 0;
	}
	else if (ast->type == NODE_CMD)
	{
		if (!ast->cmd || !ast->cmd->cmd)
		{
			fprintf(stderr, "minishell: invalid command in CMD\n");
			// shell->exit_status = 1;
			return 1;
		}

		if (buildin_checker(ast->cmd->cmd))
			find_buildin(shell,ast->cmd);
		// else
		// 	exec_external(shell, ast->cmd); // если реализовано
	}

	return 0;
}


int main(int ac, char **av, char **envp)
{
	char *line = NULL;
	t_token *token = NULL;
	t_env *env = NULL;
	t_shell shell;

	shell.env = init_env(envp);
	while (1)
	{
		// if(get_env_value(shell.env,"HOME"))
		// 	printf("%s : ", get_env_value(shell.env,"HOME"));
		char *line = readline("minishell$ ");
		if (!line)
			break;
		if(ft_flag(line, strlen(line)))
			line = read_q(line);
		if(line)
		{
			shell.tokens = parse_line(line);
			label_tokens(shell.tokens);
			shell.ast = build_ast_from_tokens(shell.tokens, NULL);
			if(shell.ast)
			{
				exec_tree(&shell, shell.ast);
				free_token_list(shell.tokens);
				free_ast(shell.ast);
			}
		}
		add_history(line);
		free(line);	
	}
	return(0);
}