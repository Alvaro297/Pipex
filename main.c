/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvamart <alvamart@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-25 13:06:41 by alvamart          #+#    #+#             */
/*   Updated: 2024-12-25 13:06:41 by alvamart         ###   ########.com      */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

static void	ft_second_child_process(t_pipex *pipex, char **argv, char **envp)
{
	int	fd;

	pipex->pid2 = fork();
	if (pipex->pid2 == 0)
	{
		fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		pipex->infile = fd;
		fill_pipex_structure(pipex, argv, envp);
		dup2(pipex->pipefd[0], STDIN_FILENO);
		dup2(pipex->outfile, STDOUT_FILENO);
		close(pipex->pipefd[0]);
		close(pipex->pipefd[1]);
		execve(pipex->path2, pipex->cmd2, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

static void	ft_first_child_process(t_pipex *pipex, char **argv, char **envp)
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
		pipex->outfile = fd;
		fill_pipex_structure(pipex, argv, envp);
		dup2(pipex->infile, STDIN_FILENO);
		dup2(pipex->pipefd[1], STDOUT_FILENO);
		close(pipex->pipefd[0]);
		close(pipex->pipefd[1]);
		execve(pipex->path1, pipex->cmd1, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex		pipex;

	if (argc != 5)
	{
		perror("Insuficientes argumentos");
		exit(EXIT_FAILURE);
	}
	if (check_files_for_pipex(argv) == -1)
		exit(EXIT_FAILURE);
	if (pipe(pipex.pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	ft_first_child_process(&pipex, argv, envp);
	ft_second_child_process(&pipex, argv, envp);
	close(pipex.pipefd[0]);
	close(pipex.pipefd[1]);
	waitpid(pipex.pid1, NULL, 0);
	waitpid(pipex.pid2, NULL, 0);
}
