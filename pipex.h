#ifndef PIPEX_H
# define PIPEX_H

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
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	char	**cmd1;
	char	**cmd2;
	char	*path1;
	char	*path2;
}	t_pipex;

int	check_files_for_pipex(char **files);
void	fill_pipex_structure(t_pipex *pipe, char **argv, char **envp);
void	ft_first_child_process(t_pipex *pipex, char **argv, char **envp);
#endif