
#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>

typedef struct s_cmd
{
	char	*path;
	char	**args;

	int		isPipe;
	int		fd[2];
}	t_cmd;


#endif
