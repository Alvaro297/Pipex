/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvamart <alvamart@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-25 13:06:58 by alvamart          #+#    #+#             */
/*   Updated: 2024-12-25 13:06:58 by alvamart         ###   ########.com      */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

static int	check_files_for_pipex_bonus(t_pipex *pipex, char **argv, int argc)
{
	if (!pipex->here_doc)
	{
		if (access(argv[1], R_OK) == -1)
		{
			perror("Error al abrir el archivo de entrada");
			return (-1);
		}
	}
	if (access(argv[argc - 1], W_OK) == -1
		&& access(argv[argc - 1], F_OK) == 0)
	{
		perror("Error al escribir en el archivo de salida");
		return (-1);
	}
	return (0);
}

static char *find_path2(char **path_splited, char **command)
{
	char	*full_path;
	char	*temp_path;
	int		i;

	i = 0;
	while (path_splited[i])
	{
		temp_path = ft_strjoin(path_splited[i], "/");
		full_path = ft_strjoin(temp_path, command[0]);
		free(temp_path);
		if (access(full_path, X_OK) == 0)
		{
			free_split(path_splited);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(path_splited);
	return (NULL);
}

static char	*find_path(char	**command, char **envp)
{
	char	*path_str;
	char	**path_splited;
	int		i;

	i = 0;
	path_str = NULL;
	path_splited = NULL;
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_str = envp[i] + 5;
			break;
		}
		i++;
	}
	if (path_str == NULL)
	{
		perror("No se encontro el path");
		exit(EXIT_FAILURE);
	}
	path_splited = ft_split(path_str, ':');
	return (find_path2(path_splited, command));
}

static void	ft_loc_memory(t_pipex *pipex, char **argv, int argc)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
		pipex->here_doc = 1;
	else
		pipex->here_doc = 0;
	if (pipex->here_doc == 1)
	{
		pipex->infile = -1;
		pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		pipex->arguments = argc - 4;
	}
	else
	{
		pipex->infile = open(argv[1], O_RDONLY);
		pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		pipex->arguments = argc - 3;
	}
	if (check_files_for_pipex_bonus(pipex, argv, argc) == -1)
		exit(EXIT_FAILURE);
	pipex->cmds = malloc(sizeof(char **) * pipex->arguments);
	pipex->paths = malloc(sizeof(char *) * pipex->arguments);
	pipex->pids = malloc(sizeof(pid_t) * pipex->arguments);
	pipex->pipefd = malloc(sizeof(int *) * pipex->arguments - 1);
}

void	fill_pipex_structure_bonus(t_pipex *pipex, char **argv, int argc, char **envp)
{
	int		i;

	i = 0;
	ft_loc_memory(pipex, argv, argc);
	while (i < pipex->arguments - 1)
	{
		pipex->pipefd[i] = malloc(sizeof(int) * 2);
		if (pipe(pipex->pipefd[i]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < pipex->arguments)
	{
		if (pipex->here_doc == 1)
			pipex->cmds[i] = ft_split(argv[i + 3], ' ');
		else
			pipex->cmds[i] = ft_split(argv[i + 2], ' ');
		pipex->paths[i] = find_path(pipex->cmds[i], envp);
		i++;
	}
}
