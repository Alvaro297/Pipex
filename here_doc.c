/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvamart <alvamart@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-28 13:30:47 by alvamart          #+#    #+#             */
/*   Updated: 2024-12-28 13:30:47 by alvamart         ###   ########.com      */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void free_split(char **split)
{
	int i;

	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static void	ft_free_pipex2(t_pipex *pipex, int i)
{
	if (pipex->paths)
    {
        i = 0;
        while (i < pipex->arguments)
        {
            if (pipex->paths[i])
                free(pipex->paths[i]);
            i++;
        }
        free(pipex->paths);
    }
    if (pipex->pipefd)
    {
        i = 0;
        while (i < pipex->arguments - 1)
        {
            if (pipex->pipefd[i])
                free(pipex->pipefd[i]);
            i++;
        }
        free(pipex->pipefd);
    }
    if (pipex->pids)
        free(pipex->pids);
}

void	ft_free_pipex(t_pipex *pipex)
{
    int	i;
    int	j;

    if (pipex->cmds)
    {
        i = 0;
        while (i < pipex->arguments)
        {
            if (pipex->cmds[i])
            {
                j = 0;
                while (pipex->cmds[i][j])
                {
                    free(pipex->cmds[i][j]);
                    j++;
                }
                free(pipex->cmds[i]);
            }
            i++;
        }
        free(pipex->cmds);
    }
	ft_free_pipex2(pipex, 0);
}

void	ft_here_doc(char *delimiter)
{
	char	*line;
	char	*new_limiter;
	int		here_doc_pipe[2];

	new_limiter = ft_strjoin(delimiter, "\n");
	if (pipe(here_doc_pipe) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	line = get_next_line(0);
	while (line)
	{
		if (ft_strncmp(line, new_limiter, ft_strlen(new_limiter)) == 0)
			break ;
		write(here_doc_pipe[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	free(new_limiter);
	free(line);
	close(here_doc_pipe[1]);
	dup2(here_doc_pipe[0], STDIN_FILENO);
	close(here_doc_pipe[0]);
}
