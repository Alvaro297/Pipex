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

int	check_files_for_pipex_bonus(char **files, int argc)
{
	if (access(files[1], R_OK) == -1)
	{
		perror("Error al abrir el archivo de entrada");
		return (-1);
	}
	if (access(files[argc-1], W_OK) == -1 && access(files[argc - 1], F_OK) == 0)
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
			return (full_path);
		free(full_path);
		i++;
	}
	perror("Comando no encontrado en el PATH");
	exit(EXIT_FAILURE);
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

static void	ft_loc_memory(t_pipex *pipe, char **argv)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
		pipe->here_doc = 1;
	else
		pipe->here_doc = 0;
	if (pipe->here_doc == 1)
	{
		pipex.infile = -1;
		pipex.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else
	{
		pipex.infile = open(argv[1], O_RDONLY);
		pipex.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (pipex.infile < 0 || pipex.outfile < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	pipe->cmds = malloc(sizeof(char **) * pipe->arguments );
	pipe->paths = malloc(sizeof(char *) * pipe->arguments);
	pipe->pids = malloc(sizeof(pid_t) * pipe->arguments);
	pipe->pipefd = malloc(sizeof(int *) * pipe->arguments - 1);
}

void	fill_pipex_structure_bonus(t_pipex *pipe, char **argv, char **envp)
{
	int		i;

	i = 0;
	ft_loc_memory(pipe, argv);
	while (i < pipe->arguments - 1)
	{
		pipe->pipefd[i] = malloc(sizeof(int) * 2);
		if (pipe(pipe->pipefd[i]) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < pipe->arguments)
	{
		pipe->cmds[i] = ft_split(argv[i + 2], ' ');
		pipe->paths[i] = find_path(pipe->cmds[i], envp);
		i++;
	}
}
