/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:44:28 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 19:52:27 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (argc == 9)
	{
		if (parsing(argv, &sim))
			return (1);
		if (init_simulation(&sim))
			return (1);
		if (start_simulation(&sim))
		{
			destroy_sync(&sim);
			free_simulation(&sim);
			return (1);
		}
		free_simulation(&sim);
		destroy_sync(&sim);
	}
	else
	{
		fprintf(stderr, "[Error]: Inputs Must Be 8\nGot: [ %d ]\n", argc - 1);
		return (1);
	}
	return (0);
}
