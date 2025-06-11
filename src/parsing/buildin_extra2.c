#include "minishell.h"

int is_n_option(const char *arg)
{
    int j;

    if (arg == NULL || arg[0] != '-' || arg[1] != 'n')
        return (0); // Должен начинаться с "-n"

    j = 2; // Начинаем проверку с символа после "-n"
    while (arg[j] != '\0')
    {
        if (arg[j] != 'n')
            return (0); // Если после "-n" есть что-то кроме 'n', это не флаг
        j++;
    }
    return (1);
}

int ft_echo(t_shell *shell, t_cmd *cmd)
{
    int i;
    int print_newline;

    (void)shell;
    i = 1; // Начинаем проверку аргументов с cmd->args[1]
    print_newline = 1; // По умолчанию: печатать символ новой строки в конце
    if (cmd->args[1] != NULL && is_n_option(cmd->args[1]))
    {
        print_newline = 0; // Подавить новую строку
        i++;               // Начать печать со следующего аргумента (cmd->args[2])
    }
    while (cmd->args[i] != NULL)
    {
        write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
        if (cmd->args[i + 1] != NULL) // Если есть еще один аргумент, печатаем пробел
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (print_newline)
        write(STDOUT_FILENO, "\n", 1);
    return (0); // Встроенная команда echo обычно возвращает 0 (успех)
}

int ft_env(t_shell *shell)
{
	t_env *current_env;

	current_env = shell->env; // Получаем начало списка переменных окружения
	while (current_env != NULL)
	{
		// Печатаем переменную только если у нее есть значение (val не NULL).
		// Если val это пустая строка "", то "KEY=" будет напечатано.
		// Если val это NULL, то переменная не печатается.
		if (current_env->val != NULL)
		{
			printf("%s=%s\n", current_env->key, current_env->val);
		}
		current_env = current_env->next; // Переходим к следующей переменной
	}
	return (0); // Команда env обычно завершается успешно
}

int ft_pwd(t_shell *shell, t_cmd *cmd)
{
    char *cwd_val; // Используем указатель для динамического выделения

    (void)cmd; // Если аргументы команды не используются для pwd

    cwd_val = getcwd(NULL, 0); // getcwd выделит память нужного размера
    if (cwd_val != NULL)
    {
        printf("%s\n", cwd_val);
        free(cwd_val); // Освобождаем память, выделенную getcwd
        if (shell) // Устанавливаем статус, если shell передан
            shell->exit_status = 0;
        return (0); // 0 - успех
    }
    else
    {
        perror("minishell: pwd"); // Более стандартное сообщение об ошибке
        if (shell) // Устанавливаем статус, если shell передан
            shell->exit_status = 1;
        return (1); // 1 - ошибка
    }
}
