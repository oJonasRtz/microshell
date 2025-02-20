
#include "microshell.h"

static void	exec_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		if (cmd->isPipe)
		{
			dup2(cmd->fd[1], 1);
			close(cmd->fd[1]);
			close(cmd->fd[0]);
		}
		execve(cmd->path, cmd->args, envp);
		exit(1);
	}
	else if (cmd->isPipe)
	{
		dup2(cmd->fd[0], 0);
		close(cmd->fd[1]);
		close(cmd->fd[0]);
	}
	waitpid(pid, NULL, 0);
}

static void	init_pipe(t_cmd *cmd)
{
	if (!cmd->isPipe)
		return ;
	if (pipe(cmd->fd) < 0)
		exit (1);
}

static void	find_cmd(char ***argv, t_cmd *cmd)
{
	int	i;

	i = 0;
	while ((*argv)[i] && strcmp((*argv)[i], "|") && strcmp((*argv)[i], ";"))
		i++;
	if ((*argv)[i] == 0)
	{
		*argv = &((*argv)[i]);
		return ;
	}
	if (!strcmp((*argv)[i], "|"))
		cmd->isPipe = 1;
	(*argv)[i] = 0;
	*argv = &((*argv)[i + 1]);
}

static int	ft_cd(t_cmd *cmd)
{
	if (strcmp(cmd->args[0], "cd"))
		return (1);
	chdir(cmd->args[1]);
	return (0);
}

static int	microshell(char **argv, char **envp)
{
	t_cmd	cmd;

	while (*argv)
	{
		cmd.path = *argv;
		cmd.args = argv;
		cmd.isPipe = 0;
		find_cmd(&argv, &cmd);
		init_pipe(&cmd);
		if (ft_cd(&cmd))
			exec_cmd(&cmd, envp);
	}
	return (0);
}

int	main (int argc, char **argv, char **envp)
{
	if (argc == 1)
		return (0);
	return (microshell(&argv[1], envp));
}
