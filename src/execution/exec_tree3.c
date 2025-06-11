#include "../inc/minishell.h"

int	open_input_redir(t_ast *node, int *fd, char *err_msg)
{
	if (!node->cmd || !node->cmd->infile)
		return (EXIT_FAILURE);
	snprintf(err_msg, 1024, "minishell: %s", node->cmd->infile);
	*fd = open(node->cmd->infile, O_RDONLY);
	return (0);
}

int	open_output_redir(t_ast *node, int *fd, char *err_msg)
{
	int	flags;

	if (!node->cmd || !node->cmd->outfile)
		return (EXIT_FAILURE);
	flags = O_WRONLY | O_CREAT
		| (node->type == NODE_REDIR_APPEND ? O_APPEND : O_TRUNC);
	snprintf(err_msg, 1024, "minishell: %s", node->cmd->outfile);
	*fd = open(node->cmd->outfile, flags, 0644);
	return (0);
}

int	do_dup_and_close(int fd, int target, char *err_msg, int *to_close)
{
	if (fd < 0)
	{
		perror(err_msg);
		return (1);
	}
	if (dup2(fd, target) == -1)
	{
		perror(target == STDIN_FILENO
			? "minishell: dup2 stdin"
			: "minishell: dup2 stdout");
		close(fd);
		return (EXIT_FAILURE);
	}
	*to_close = fd;
	return (0);
}

int	handle_heredoc_redir(t_shell *shell, t_ast *node)
{
	if (!node->cmd || !node->cmd->heredoc)
		return (EXIT_FAILURE);
	return (setup_heredoc_input(shell, node->cmd->heredoc));
}


int	apply_single_redirection(t_shell *shell,
			t_ast *redir_node, int *fd_to_close_later)
{
	int		fd;
	int		target_fileno;
	char	err_msg[1024];

	fd = -1;
	target_fileno = -1;
	*fd_to_close_later = -1;
	if (redir_node->type == NODE_REDIR_IN)
	{
		if (open_input_redir(redir_node, &fd, err_msg))
			return (EXIT_FAILURE);
		target_fileno = STDIN_FILENO;
	}
	else if (redir_node->type == NODE_REDIR_OUT
		|| redir_node->type == NODE_REDIR_APPEND)
	{
		if (open_output_redir(redir_node, &fd, err_msg))
			return (EXIT_FAILURE);
		target_fileno = STDOUT_FILENO;
	}
	else if (redir_node->type == NODE_HEREDOC)
		return (handle_heredoc_redir(shell, redir_node));
	return (do_dup_and_close(fd, target_fileno, err_msg, fd_to_close_later));
}
