/*
 * ________________________________________________________________________________
 * |  File: io.c
 * |  Project: source
 * |  File Created: Wednesday, 20th November 2024 10:47 am
 * |  Author: Daniel Haddington [haddingtondaniel5@icloud.com]
 */

#include "../includes/yosh_i.h"

int	in(t_cmd *cmd, int i)
{
	if (is_valid_filename(cmd->tokens[i + 1]))
	{
		if (cmd->io_cross_in)
			return (0);
		if (cmd->io_dup_in)
			return (0);
		cmd->io_dup_in = 1;
		cmd->fd_stdin = dup(STDIN_FILENO);
		cmd->fd_infile = open(cmd->tokens[i + 1], O_RDONLY);
		if (cmd->fd_infile == -1)
			perror("Error: INFILE OPEN");
		dup2(cmd->fd_infile, STDIN_FILENO);
		return (1);
	}
	return (0);
}

int	in_here(t_cmd *cmd, int i, t_norm *norm)
{
	if (cmd->io_cross_in)
		return (0);
	if (cmd->io_dup_in)
		return (0);
	cmd->io_dup_in = 1;
	cmd->fd_stdin = dup(STDIN_FILENO);
	cmd->here_prompt = strdup("writenow > ");
	cmd->here_eof = strdup(cmd->tokens[i + 1]);
	cmd->fd_here = open(cmd->temporary_heredoc,
			O_CREAT | O_RDWR | O_TRUNC, 0700);
	if (cmd->fd_here == -1)
		perror("Error: INFILE OPEN");
	write_2_fd_here(cmd, norm);
	lseek(cmd->fd_here, 0, SEEK_SET);
	dup2(cmd->fd_here, STDIN_FILENO);
	return (1);
}

void	write_2_fd_here(t_cmd *cmd, t_norm *norm)
{
	cmd->here_buffer = malloc(1);
	while (strcmp(cmd->here_buffer, cmd->here_eof) != 0)
	{
		cmd->here_buffer = readline(cmd->here_prompt);
		if (cmd->here_buffer)
		{
			if (ft_strcmp(cmd->here_buffer, cmd->here_eof) == 0)
				break ;
			write(cmd->fd_here, cmd->here_buffer, ft_strlen(cmd->here_buffer));
			write(cmd->fd_here, "\n", 1);
		}
		else
			exitshell(cmd, norm);
	}
	free(cmd->here_buffer);
}

int	out(t_cmd *cmd, int i)
{
	if (is_valid_filename(cmd->tokens[i + 1]))
	{
		if (cmd->io_dup_out)
			return (0);
		cmd->io_dup_out = 1;
		cmd->fd_stdout = dup(STDOUT_FILENO);
		cmd->fd_outfile = open(cmd->tokens[i + 1],
				O_CREAT | O_WRONLY | O_TRUNC, 0700);
		if (cmd->fd_outfile == -1)
			perror("Error: OUTFILE OPEN");
		dup2(cmd->fd_outfile, STDOUT_FILENO);
		return (1);
	}
	return (0);
}

int	out_append(t_cmd *cmd, int i)
{
	if (is_valid_filename(cmd->tokens[i + 1]))
	{
		if (cmd->io_dup_out)
			return (0);
		cmd->io_dup_out = 1;
		cmd->fd_stdout = dup(STDOUT_FILENO);
		cmd->fd_outfile = open(cmd->tokens[i + 1],
				O_CREAT | O_WRONLY | O_APPEND, 0700);
		if (cmd->fd_stdout == -1)
			perror("Error: OUTFILE OPEN");
		dup2(cmd->fd_outfile, STDOUT_FILENO);
		return (1);
	}
	return (0);
}