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
	int		infile;     // Descriptor de archivo para el archivo de entrada
	int		outfile;    // Descriptor de archivo para el archivo de salida
	int		pipefd[2];  // Descriptores de archivo para el pipe
	pid_t	pid1;       // ID del primer proceso hijo
	pid_t	pid2;       // ID del segundo proceso hijo
	char	**cmd1;     // Comando y argumentos para el primer proceso
	char	**cmd2;     // Comando y argumentos para el segundo proceso
	char	*path1;     // Ruta del ejecutable del primer comando
	char	*path2;     // Ruta del ejecutable del segundo comando
}	t_pipex;

int	check_files_for_pipex(char **files);

#endif