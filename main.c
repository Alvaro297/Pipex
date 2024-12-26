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

void print_pipex_structure(t_pipex *pipex)
{
    int i;

    printf("infile: %d\n", pipex->infile);
    printf("outfile: %d\n", pipex->outfile);
    printf("pipefd[0]: %d\n", pipex->pipefd[0]);
    printf("pipefd[1]: %d\n", pipex->pipefd[1]);
    printf("pid1: %d\n", pipex->pid1);
    printf("pid2: %d\n", pipex->pid2);

    printf("cmd1: ");
    if (pipex->cmd1)
    {
        i = 0;
        while (pipex->cmd1[i])
        {
            printf("%s ", pipex->cmd1[i]);
            i++;
        }
    }
    else
    {
        printf("(null)");
    }
    printf("\n");

    printf("cmd2: ");
    if (pipex->cmd2)
    {
        i = 0;
        while (pipex->cmd2[i])
        {
            printf("%s ", pipex->cmd2[i]);
            i++;
        }
    }
    else
    {
        printf("(null)");
    }
    printf("\n");

    printf("path1: %s\n", pipex->path1 ? pipex->path1 : "(null)");
    printf("path2: %s\n", pipex->path2 ? pipex->path2 : "(null)");
}

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
		pipex->outfile = fd;
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
		pipex->infile = fd;
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
	return (0);
}
