/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:50:17 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/28 12:09:27 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/// REMAKE by Baubek////

int exec_redir_chain(t_shell *shell, t_ast *ast)
{
    t_ast   *cmd_node_candidate = ast;
    int     original_fds[2] = {-1, -1};
    int     status = EXIT_SUCCESS; // Изначально успех
    t_ast   *redir_stack[64];
    int     stack_depth = 0;
    int     opened_fds[64]; // Для хранения fd, открытых для редиректов
    int     opened_fds_count = 0;
    int     i;

    original_fds[0] = dup(STDIN_FILENO);
    original_fds[1] = dup(STDOUT_FILENO);
    if (original_fds[0] < 0 || original_fds[1] < 0) {
        perror("minishell: redir_chain: dup original fds");
        if (original_fds[0] >= 0) close(original_fds[0]);
        if (original_fds[1] >= 0) close(original_fds[1]);
        return (EXIT_FAILURE);
    }

    while (cmd_node_candidate != NULL && (cmd_node_candidate->type == NODE_REDIR_OUT ||
        cmd_node_candidate->type == NODE_REDIR_APPEND ||
        cmd_node_candidate->type == NODE_REDIR_IN ||
        cmd_node_candidate->type == NODE_HEREDOC))
    {
        if (stack_depth >= 63) {
            write_error("minishell: ", "too many redirections", NULL);
            status = EXIT_FAILURE; restore_original_fds(original_fds[0], original_fds[1]); return status; }
        redir_stack[stack_depth++] = cmd_node_candidate;
        cmd_node_candidate = cmd_node_candidate->left;
    }

    for (i = stack_depth - 1; i >= 0; i--)
    {
        int fd_to_close_this_iter = -1;
        if (apply_single_redirection(shell, redir_stack[i], &fd_to_close_this_iter) != 0)
        {
            status = (errno == EACCES || errno == ENOENT) ? 1 : EXIT_FAILURE; // Более точный статус
            // Закрыть все ранее открытые fd для редиректов в этой цепочке
            for (int k = 0; k < opened_fds_count; k++) close(opened_fds[k]);
            restore_original_fds(original_fds[0], original_fds[1]);
            return status;
        }
        if (fd_to_close_this_iter >= 0) // Если apply_single_redirection открыл файл
        {
            if (opened_fds_count < 64) opened_fds[opened_fds_count++] = fd_to_close_this_iter;
            else { /* Слишком много открытых файлов, обработка ошибки */ close(fd_to_close_this_iter); }
        }
    }

    status = exec_tree(shell, cmd_node_candidate);

    // Закрыть все файловые дескрипторы, открытые для редиректов
    for (i = 0; i < opened_fds_count; i++)
        close(opened_fds[i]);

    restore_original_fds(original_fds[0], original_fds[1]);
    return status;
}


int exec_redir_node(t_shell *shell, t_ast *ast)
{
	int fd;
	int saved_fd;
	int status;
	char *filename = NULL;
	int is_output = (ast->type == NODE_REDIR_OUT || ast->type == NODE_REDIR_APPEND);
	int flags = is_output ? O_WRONLY | O_CREAT | (ast->type == NODE_REDIR_APPEND ? O_APPEND : O_TRUNC) : O_RDONLY;

	filename = is_output ? ast->cmd->outfile : ast->cmd->infile;
	if (!filename || !ast->left)
		return (write_error("minishell: redirection error", NULL, NULL), EXIT_FAILURE);
	fd = open(filename, flags, 0644);
	if (fd < 0)
		return (perror(filename), EXIT_FAILURE);
	saved_fd = dup(is_output ? STDOUT_FILENO : STDIN_FILENO);
	dup2(fd, is_output ? STDOUT_FILENO : STDIN_FILENO);
	close(fd);
	status = exec_tree(shell, ast->left);
	dup2(saved_fd, is_output ? STDOUT_FILENO : STDIN_FILENO);
	close(saved_fd);
	return (status);
}

int exec_heredoc_node(t_shell *shell, t_ast *ast)
{
	int pipefd[2];
	char *line = NULL;

	if (!ast->cmd || !ast->cmd->heredoc || !ast->left)
		return (write_error("minishell: heredoc error", NULL, NULL), EXIT_FAILURE);
	if (pipe(pipefd) == -1)
		return (perror("pipe"), EXIT_FAILURE);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, ast->cmd->heredoc) == 0)
			break;
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipefd[1]);
	int saved_stdin = dup(STDIN_FILENO);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	int status = exec_tree(shell, ast->left);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	return (status);
}

int exec_cmd_node(t_shell *shell, t_ast *ast)
{
	int status;
	if (!ast->cmd || !ast->cmd->cmd)
		return (write_error("minishell: invalid command", NULL, NULL), 127);
	if (buildin_checker(ast->cmd->cmd))
		status = find_buildin(shell, ast->cmd);
	else
		status = exec_external(shell, ast->cmd);
	return (status);
}
