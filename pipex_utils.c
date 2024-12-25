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
#include "pipex.h"

int	check_files_for_pipex(char **files)
{
	if (access(argv[1], R_OK) == -1)
	{
		perror("Error al abrir el archivo de entrada");
		return (-1);
	}
	if (access(argv[4], W_OK) == -1 && access(argv[4], F_OK) == 0)
	{
		perror("Error al escribir en el archivo de salida");
		return (-1);
	}
	return (0);
}

static char	*find_path(char	*command, char **envp)
{
	char	*path_str;
	char	**path_splited;
	char	*full_path;
	int		i;

	i = 0;
	path_str = NULL;
	path_splited = NULL;
	while (envp[i] && path_str == NULL)
		path_str = ft_strnstr(envp[i++], "PATH=", 5);
	if (path_str == NULL)
	{
		perror("No se encontro el path");
		exit(EXIT_FAILURE);
	}
	path_splited = ft_split(path_str, ":");
	i = 0;
	while (path_splited[i])
	{
		full_path = ft_strjoin(path_splited[i], "/");
		full_path = ft_strjoin(full_path, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	perror("Comando no encontrado en el PATH");
	exit(EXIT_FAILURE);
}

void	fill_pipex_structure(t_pipex *pipe, char **argv, char **envp)
{
	pipe->path1 = find_path(argv[2], envp);
	pipe->path2 = find_path(argv[3], envp);
	pipe->cmd1 = ft_split(argv[2]);
	pipe->cmd2 = ft_split(argv[3]);
}
