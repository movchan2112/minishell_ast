#include "../inc/minishell.h"

int	exec_pipe_node(t_shell *shell, t_ast *ast)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), EXIT_FAILURE);
	pid1 = fork();
	if (pid1 < 0)
		return (perror("minishell: fork"), EXIT_FAILURE);
	if (pid1 == 0)
		exec_child_left(shell, ast, pipefd);
	pid2 = fork();
	if (pid2 < 0)
		return (perror("minishell: fork"), EXIT_FAILURE);
	if (pid2 == 0)
		exec_child_right(shell, ast, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	update_exit_status(shell, status2);
	return (shell->exit_status);
}


int	print_heredoc_warning(char *delimiter)
{
    write_error(
	"minishell: warning: here-document delimited by end-of-file (wanted `",
	delimiter,
	"`)\n");
	return (1);
}

int	write_heredoc_loop(int write_fd,char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (print_heredoc_warning(delimiter));
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	return (0);
}

int	setup_heredoc_input(t_shell *shell,char *delimiter)
{
	int	heredoc_pipefd[2];

	(void)shell;
	if (pipe(heredoc_pipefd) == -1)
		return (perror("minishell: heredoc: pipe"), 1);
	if (write_heredoc_loop(heredoc_pipefd[1], delimiter))
	{
		close(heredoc_pipefd[0]);
		close(heredoc_pipefd[1]);
		return (1);
	}
	close(heredoc_pipefd[1]);
	if (dup2(heredoc_pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: heredoc: dup2 stdin");
		close(heredoc_pipefd[0]);
		return (1);
	}
	close(heredoc_pipefd[0]);
	return (0);
}

static void restore_original_fds(int original_stdin, int original_stdout)
{
    if (original_stdin >= 0) {
        if (dup2(original_stdin, STDIN_FILENO) == -1)
            perror("minishell: redir_chain: restore stdin failed");
        close(original_stdin);
    }
    if (original_stdout >= 0) {
        if (dup2(original_stdout, STDOUT_FILENO) == -1)
            perror("minishell: redir_chain: restore stdout failed");
        close(original_stdout);
    }
}