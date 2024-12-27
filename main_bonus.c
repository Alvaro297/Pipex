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
#include "pipex_bonus.h"

static void	ft_last_child_process(t_pipex *pipex, char **envp)
{
	dup2(pipex->pipefd[pipex->arguments - 2][0], STDIN_FILENO);
	dup2(pipex->outfile, STDOUT_FILENO);
	execve(pipex->paths[pipex->arguments - 1],
		pipex->cmds[pipex->arguments - 1], envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

static void	ft_first_child_process(t_pipex *pipex, char **envp)
{
	dup2(pipex->infile, STDIN_FILENO);
	dup2(pipex->pipefd[0][1], STDOUT_FILENO);
	execve(pipex->paths[0], pipex->cmds[0], envp);
	perror("execve");
	exit(EXIT_FAILURE);
}
static void	execute_command(t_pipex *pipex, int index, char **envp)
{
	dup2(pipex->pipefd[index - 1][0], STDIN_FILENO);
	dup2(pipex->pipefd[index][1], STDOUT_FILENO);
	execve(pipex->paths[index], pipex->cmds[index], envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

static void	execute_all(t_pipex *pipex, char **envp, int i)
{
	while (i < pipex->arguments)
	{
		pipex.pids[i] = fork();
		if (pipex.pids[i] == 0)
		{
			if (i == 0)
				ft_first_child_process(pipex, envp);
			else if (i == pipex->arguments - 1)
				ft_last_child_process(pipex, envp);
			else
				execute_command(&pipex, i, envp);
		}
		i++;
	}
	i = 0;
	while (i < pipex->arguments - 1)
	{
		close(pipex.pipefd[i][0]);
		close(pipex.pipefd[i][1]);
		i++;
	}
	i = 0;
	while (i < pipex->arguments)
		waitpid(pipex->pids[i++], NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex		pipex;

	if (argc < 5)
	{
		perror("Insuficientes argumentos");
		exit(EXIT_FAILURE);
	}
	if (check_files_for_pipex_bonus(argv, argc) == -1)
		exit(EXIT_FAILURE);
	if (pipe(pipex.pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pipex.arguments = argc - 3;
	fill_pipex_structure_bonus(&pipex, argv, envp);
	execute_all(&pipex, argv, 0);
	return (0);
}
