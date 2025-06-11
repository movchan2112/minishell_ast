#include "minishell.h"

static int	resolve_home(t_shell *shell, const char **target, char *oldpwd)
{
	const char	*path;

	path = get_env_value(shell->env, "HOME");
	if (!path)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		free(oldpwd);
		return (1);
	}
	if (*path == '\0')
	{
		fprintf(stderr, "minishell: cd: HOME is empty\n");
		free(oldpwd);
		return (1);
	}
	*target = path;
	return (0);
}

static int	resolve_oldpwd(t_shell *shell, const char **target, char *oldpwd)
{
	const char	*path;

	path = get_env_value(shell->env, "OLDPWD");
	if (!path)
	{
		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
		free(oldpwd);
		return (1);
	}
	if (*path == '\0')
	{
		fprintf(stderr, "minishell: cd: OLDPWD is empty\n");
		free(oldpwd);
		return (1);
	}
	printf("%s\n", path);
	*target = path;
	return (0);
}

static int	resolve_target_path(t_shell *shell,
		t_cmd *cmd, const char **target, char *oldpwd)
{
	if (!cmd->args[1])
		return (resolve_home(shell, target, oldpwd));
	if (ft_strcmp(cmd->args[1], "-") == 0)
		return (resolve_oldpwd(shell, target, oldpwd));
	*target = cmd->args[1];
	return (0);
}

static int	do_chdir_and_update(t_shell *shell,
		char *oldpwd, const char *target)
{
	char	*newpwd;

	if (chdir(target) != 0)
	{
		perror("minishell: cd: failed");
		free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("minishell: cd: getcwd failed");
		change_env_value(&shell->env, "OLDPWD", oldpwd);
		free(oldpwd);
		return (1);
	}
	if (oldpwd)
		change_env_value(&shell->env, "OLDPWD", oldpwd);
	change_env_value(&shell->env, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}

int	ft_cd(t_shell *shell, t_cmd *cmd)
{
	char		*oldpwd;
	const char	*target;
	int			status;

	oldpwd = get_oldpwd();
	if (check_cd_args(cmd, &oldpwd, shell))
		return (1);
	if (resolve_target_path(shell, cmd, &target, oldpwd))
		return (1);
	status = do_chdir_and_update(shell, oldpwd, target);
	return (status);
}
