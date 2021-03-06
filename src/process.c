/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cproesch <cproesch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:24:39 by cproesch          #+#    #+#             */
/*   Updated: 2021/11/02 12:11:23 by cproesch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check_file1(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		perror("error - open file1 failed");
	return (fd);
}

int	check_file2(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		perror("error - open file2 failed");
	return (fd);
}

char	**parse_cmd(char *argv)
{
	int		i;
	char	**tab;

	i = 0;
	tab = ft_split(argv, ' ');
	if (!tab)
		return (NULL);
	while (tab[i])
		i++;
	tab[i] = NULL;
	if (!tab)
		perror("cmd path error");
	return (tab);
}

void	child1_process(t_data *data, char **argv, char **envp)
{
	close(data->pipefd[0]);
	data->input = check_file1(argv[1]);
	if (data->input == -1)
		ft_exit(&data, 2);
	dup2(data->input, STDIN_FILENO);
	close(data->input);
	dup2(data->pipefd[1], STDOUT_FILENO);
	close(data->pipefd[1]);
	data->cmd1 = parse_cmd(argv[2]);
	data->cmd1_path = find_path(envp, data->cmd1[0]);
	if (!data->cmd1 || !(data->cmd1_path))
		ft_exit(&data, 2);
	if (execve(data->cmd1_path, data->cmd1, envp) == -1)
		perror("error - Second exceve failed");
}

void	child2_process(t_data *data, char **argv, char **envp)
{
	close(data->pipefd[1]);
	data->output = check_file2(argv[4]);
	if (data->output == -1)
		ft_exit(&data, 2);
	dup2(data->output, STDOUT_FILENO);
	close(data->output);
	dup2(data->pipefd[0], STDIN_FILENO);
	close(data->pipefd[0]);
	data->cmd2 = parse_cmd(argv[3]);
	data->cmd2_path = find_path(envp, data->cmd2[0]);
	if (!data->cmd2 || !(data->cmd2_path))
		ft_exit(&data, 2);
	if (execve(data->cmd2_path, data->cmd2, envp) == -1)
		perror("error - First exceve failed");
}
