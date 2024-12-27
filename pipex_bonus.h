#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft_pipex/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

# define EXIT_FAILURE 1

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		arguments;
	int		**pipefd;
	pid_t	*pid;
	char	***cmds;
	char	**paths;
	int		here_doc;
}	t_pipex;

int	main(int argc, char **argv, char **envp);
int	check_files_for_pipex_bonus(char **files, int argc);
void	fill_pipex_structure_bonus(t_pipex *pipe, char **argv, char **envp);
#endif