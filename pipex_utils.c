# include "pipex.h"

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

char	*find_path(char	*command)
{
	
}

void	fill_pipex_structure(t_pipex *pipe, char **argv)
{
	pipe->path1 = find_path(argv[2]);
	pipe->path2 = find_path(argv[3]);
	pipe->cmd1 = ft_split(argv[2]);
	pipe->cmd2 = ft_split(argv[3]);
}