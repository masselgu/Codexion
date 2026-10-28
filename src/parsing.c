/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:43:53 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 19:54:18 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parsing(char **av, t_sim *sim)
{
	if (validate_args(av) == 1)
		return (1);
	sim->number_of_coders = ft_atoi(av[1]);
	sim->time_to_burnout = ft_atoi(av[2]);
	sim->time_to_compile = ft_atoi(av[3]);
	sim->time_to_debug = ft_atoi(av[4]);
	sim->time_to_refactor = ft_atoi(av[5]);
	sim->number_of_compiles_required = ft_atoi(av[6]);
	sim->dongle_cooldown = ft_atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		sim->scheduler_mode = 0;
	else if (strcmp(av[8], "edf") == 0)
		sim->scheduler_mode = 1;
	else
	{
		fprintf(stderr, "[Error]: Expecting "
			"'edf' or 'fifo'\nGot: [ %s ]\n", av[8]);
		return (1);
	}
	return (0);
}

int	validate_args(char **av)
{
	int		i;

	i = 1;
	while (i <= 7)
	{
		if (validate_number(av, i))
			return (1);
		if (validate_value(av, i))
			return (1);
		i++;
	}
	return (0);
}

int	validate_number(char **av, int index)
{
	long	value;

	if (valid_number(av[index]))
	{
		fprintf(stderr, "[Error]: No Number or Negative Number or Malformed Number.\n");
		return (1);
	}
	value = ft_atoi(av[index]);
	if (value == -1)
	{
		fprintf(stderr, "[Error]: Value Overflowed.\n");
		return (1);
	}
	return (0);
}

int	validate_value(char **av, int index)
{
	long	value;

	value = ft_atoi(av[index]);
	if (index == 1 || index == 6)
	{
		if (index == 1)
		{
			if (value == 0)
			{
				fprintf(stderr, "[Error]: Coders Must Be More Than 0.\n");
				return (1);
			}
		}
	}
	return (0);
}
