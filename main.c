#include "pipex.h"

void	ft_first_child_process(t_pipex *pipex, char **argv)
{
	int	fd;

	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		pipex->infile = fd;
		fill_pipex_structure(pipex, argv);
		dup2(pipex->infile, STDIN_FILENO);
		dup2(pipex->pipefd[1], STDOUT_FILENO);
		close(pipex->pipefd[0]);
		close(pipex->pipefd[1]);
		execve(pipex->path1, pipex->cmd1, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

int		main(int argc, char **argv)
{
	t_pipex		pipex;

	if (argc != 5)
	{
		perror("Insuficientes argumentos");
		exit(EXIT_FAILURE);
	}
	if (check_files_for_pipex(argv) == -1)
		exit(EXIT_FAILURE);
	if (pipe(pipe.pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	ft_first_child_process(&pipex, argv);
}
