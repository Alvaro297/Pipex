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
	pid_t	*pids;
	char	***cmds;
	char	**paths;
	int		here_doc;
}	t_pipex;

int		main(int argc, char **argv, char **envp);
void	fill_pipex_structure_bonus(t_pipex *pipe, char **argv, int argc, char **envp);
void	ft_here_doc(char *delimiter);
void	ft_free_pipex(t_pipex *pipex);
void 	free_split(char **split);
#endif