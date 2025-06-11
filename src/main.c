/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:04:26 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/28 13:54:01 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Эту переменную лучше объявить как static в файле, где находятся обработчики
// и основной цикл, если она не нужна в других файлах.
// 'volatile sig_atomic_t' - типы, безопасные для использования в обработчиках сигналов.

volatile sig_atomic_t	g_received_signal = 0;


t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value != NULL)
		node->val = ft_strdup(value);
	else
		node->val = NULL;
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env_list, const char *key, const char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = new_env_node(key, value);
	if (!new_node)
		return ;
	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

/**
* @brief Processes a single environment string (e.g., "KEY=value")
* and adds it to the environment list.
*
* @param env_str The environment string to process.
* @param head Pointer to the head of the environment list.
*/
static void	process_env_entry(char *env_str, t_env **head)
{
	char	*delimiter_pos;
	char	*key;
	char	*val;
	int		key_len;

	delimiter_pos = ft_strchr(env_str, '=');
	if (delimiter_pos)
	{
		key_len = delimiter_pos - env_str;
		key = ft_strndup(env_str, key_len);
		if (!key)
		{
			free(key);
			return ;
		}
		val = ft_strdup(delimiter_pos + 1);
		if (!val)
		{
			free(key);
			return ;
		}
		add_env_node(head, key, val);
		free(key);
		free(val);
	}
}

/**
* @brief Initializes the environment list from an array of strings.
* Each string is expected to be in the format "KEY=value".
*
* @param envp Array of environment strings.
* @return Pointer to the head of the created environment list.
*/
t_env	*init_env(char **envp)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		process_env_entry(envp[i], &head);
		i++;
	}
	return (head);
}

/*
 * @brief Initializes the shell structure.
 * @param shell The shell structure to initialize.
 * @param envp The environment variables from main.
 */
void	init_shell_data(t_shell *shell, char **envp)
{
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->env = init_env(envp);
	shell->exit_status = 0;
}

/*
 * @brief Processes a single command line: parsing, AST building, execution.
 * @param shell The shell structure.
 * @param line The command line string to process (already preprocessed).
 */

static void	process_command_line(t_shell *shell, char *line)
{
	if (!line || !*line)
		return ;
	shell->tokens = parse_line(line, shell);
	if (!shell->tokens)
	{
		// shell->exit_status = 1;
		return ;
	}
	label_tokens(shell->tokens);
	shell->ast = build_ast_from_tokens(shell->tokens, NULL);
	if (shell->ast)
	{
		// print_ast(shell->ast,0);
		shell->exit_status = exec_tree(shell, shell->ast);
	}
	free_command_resources(shell);
}
/*
 * @brief Handles potentially multi-line input if quotes are open.
 * Takes ownership of current_line if it's extended (frees it).
 * @param current_line The initial line read from readline.
 * @return The complete line (possibly new, possibly original),
 * or NULL on error/EOF in read_q.
 */

static char	*get_complete_line(char *current_line)
{
	char	*extended_line;

	if (!current_line)
		return (NULL);
	if (ft_flag(current_line, ft_strlen(current_line)))
	{
		extended_line = read_q(current_line);
		free(current_line);
		return (extended_line);
	}
	return (current_line);
}

// --- ОБРАБОТЧИКИ СИГНАЛОВ ---
static void handle_sigint_interactive(int signo)
{
	(void)signo;
	g_received_signal = SIGINT;
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
	// НЕ вызывайте здесь rl_functions или printf/write напрямую
}

/*
 * @brief Main loop of the minishell.
 * @param shell The shell structure.
 */

// --- ОСНОВНОЙ ЦИКЛ ОБОЛОЧКИ ---
static void minishell_loop(t_shell *shell)
{
    char    *line_read;
    char    *line_to_process;

    while (1)
    {
        g_received_signal = 0; // Сброс флага перед каждым новым вводом
        // Убедитесь, что промпт здесь правильный и не дублируется
        if (g_received_signal == SIGINT)
        {
            shell->exit_status = 130; // Стандартный статус для Ctrl+C
            write(STDOUT_FILENO, "\n", 1); // Новая строка для чистоты
            if (line_read)
                free(line_read);    // Освобождаем строку, если readline что-то вернул
            continue;               // К новому промпту
        }
        line_read = readline("minishell$ ");


        if (!line_read) // Ctrl+D (EOF)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }

        if (*line_read == '\0') // Пустая строка
        {
            free(line_read);
            continue;
        }

        add_history(line_read); // Добавляем непустую строку в историю

        line_to_process = get_complete_line(line_read);

        if (line_to_process)
        {
            process_command_line(shell, line_to_process);
            if (line_to_process != line_read) // Если get_complete_line вернул новую строку
                free(line_to_process);
        }
        // Если get_complete_line вернул line_read, то он будет освобожден ниже
        // (или уже освобожден, если line_to_process был NULL и line_read не line_to_process)
        // Это требует аккуратного управления памятью.
        // Если line_to_process == line_read, то free(line_read) ниже.
        // Если line_to_process != line_read, то line_read УЖЕ free в get_complete_line.
        // В таком случае free(line_read) ниже вызовет ошибку.

        // Упрощенное управление памятью для line_read:
        // get_complete_line должен четко документировать, освобождает ли он line_read.
        // Если get_complete_line всегда освобождает переданный current_line, если возвращает новую,
        // и возвращает current_line, если не меняет, то:
        if (line_to_process == line_read && line_to_process != NULL) {
             // Только если get_complete_line вернул оригинальный указатель
             // и он не был обработан/освобожден в process_command_line
             // Но process_command_line получает line_to_process, а не line_read.
             // Эта часть сложна без знания, как работает read_q и parse_line с памятью.
             // Безопаснее всего, если process_command_line не освобождает свой аргумент `line`.
        }
        // line_read освобождается только если get_complete_line не вернул новую строку ИЛИ
        // если line_to_process стал NULL, а line_read - нет.
        // Лучше: free(line_read) если line_to_process != line_read и line_to_process != NULL.
        // А если line_to_process == line_read, то он будет освобожден в конце.
        // Оставляем как было в вашем коде (т.е. нет явного free(line_read) здесь,
        // полагаясь на free(processed_line) в вашей первоначальной версии,
        // что означает, что line_to_process должен быть освобожден, если он не NULL).
        // В моем коде выше (до этого комментария) `free(processed_line)` было в process_command_line
        // через free_command_resources.
        // Для ясности, если line_to_process был результатом get_complete_line, и он не равен line_read,
        // то line_read уже освобожден. Если line_to_process равен line_read, то он будет
        // использован и затем (в идеале) освобожден после process_command_line.
        // В вашем коде `process_command_line` вызывает `free_command_resources`, который
        // должен освобождать токены/AST, но не саму строку `line`.
        // Значит, строку, переданную в `process_command_line`, нужно освобождать после.
        // А строку `line_read` - только если она не та же, что `line_to_process`.
        // Это сложно. Самый простой вариант:

        if (line_to_process && line_to_process != line_read) {
            // line_read был освобожден в get_complete_line, line_to_process - новая строка
            // process_command_line ее обработает, а затем ее надо освободить
            // (если process_command_line сама не освобождает)
        } else if (line_to_process && line_to_process == line_read) {
            // line_to_process - это оригинальный line_read
            // process_command_line его обработает, затем его надо освободить
        } else if (!line_to_process && line_read) {
            // get_complete_line вернул NULL, но line_read был, значит он был освобожден в get_complete_line
            // (или read_q вернул NULL и current_line (т.е. line_read) был освобожден)
        }

        // Безопаснее всего так: get_complete_line отвечает за line_read.
        // Если get_complete_line вернул новую строку, он освободил старую.
        // Если вернул старую, мы ее не трогаем, ее обработает process_command_line
        // и затем мы ее освободим.
        // Если вернул NULL, старая строка должна быть освобождена в get_complete_line.
        // Текущая `minishell_loop` уже имеет `free(processed_line)` в вашем коде, что корректно,
        // если `processed_line` - это то, что было обработано.
        // Я верну `free(line_to_process)` в конце, предполагая, что `get_complete_line`
        // управляет `line_read` если создает новую строку.
        if(line_to_process)
            free(line_to_process); // Освобождаем то, что вернул get_complete_line
                                   // (если это была новая строка, старая уже free;
                                   // если это была старая, то освобождаем ее).
                                   // Это предполагает, что add_history и process_command_line
                                   // делают свои копии, если им нужно. Но add_history так не делает.
                                   // Поэтому add_history(line_read) лучше.
    }
}


// --- ГЛАВНАЯ ФУНКЦИЯ ---
int main(int ac, char **av, char **envp)
{
    t_shell         shell_data;
    struct sigaction sa_int;
    struct sigaction sa_quit;

    (void)ac;
    (void)av;

    // Настройка обработчика для SIGINT (Ctrl+C)
    sa_int.sa_handler = handle_sigint_interactive;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0; // НЕ используем SA_RESTART, чтобы readline прерывался
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("minishell: sigaction SIGINT failed");
        exit(EXIT_FAILURE);
    }

    // Настройка обработчика для SIGQUIT (Ctrl+\) - игнорировать
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
        perror("minishell: sigaction SIGQUIT failed");
        exit(EXIT_FAILURE);
    }

    init_shell_data(&shell_data, envp);
    minishell_loop(&shell_data);

    if (shell_data.env)
        ft_free_env(shell_data.env);
    rl_clear_history(); // Раскомментируйте, если хотите очищать историю при выходе

    return (shell_data.exit_status);
}
