/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_kostrik.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svydrina <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:17:17 by svydrina          #+#    #+#             */
/*   Updated: 2024/05/22 19:06:39 by svydrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void	ft_error(char *err)
{
	while(*err)
		write(2, err++, 1);
}

int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	pip[2];
	int	in = dup(STDIN_FILENO);

	i = 0;
	while (argc && argv[i] && argv[i + 1])
	{
		for (argv = &argv[i + 1], i = 0; argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"); i++);
		if (i > 0 && !strcmp(argv[0], "cd") && (i != 2 || (i == 2 && argv[1][0] == '-')))
		       ft_error("error: cd: bad arguments\n");
		else if (i > 0 && !strcmp(argv[0], "cd") && chdir(argv[1]))
		{
			ft_error("error: cd: cannot change directory to ");
			ft_error(argv[1]);
			ft_error("\n");
		}
		else if (i > 0 && strcmp(argv[0], "cd") && (!argv[i] || !strcmp(argv[i], "|") || !strcmp(argv[i], ";")) && !pipe(pip))
		{
			if (!fork())
			{
			dup2(in, STDIN_FILENO);
			close(in);
			if (argv[i] && !strcmp(argv[i], "|"))
				dup2(pip[1], STDOUT_FILENO);
			close(pip[1]);
			close(pip[0]);
			argv[i] = NULL;
			execve(argv[0], argv, envp);
			ft_error("error: cannot execute ");
			ft_error(argv[0]);
			ft_error("\n");
			}
			close(in);
			in = pip[0];
			close(pip[1]);
			waitpid(-1, NULL, WUNTRACED);
		}
	}
	close(in);
	return (0);
}
			
