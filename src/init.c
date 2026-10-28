/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:44:34 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:44:35 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_coders(t_sim *sim, int count)
{
	while (count > 0)
	{
		count--;
		pthread_mutex_destroy(&sim->coders[count].c_mutex);
	}
}

int	init_coder(t_sim *sim, int i)
{
	sim->coders[i].id = i + 1;
	sim->coders[i].compile_count = 0;
	sim->coders[i].data = sim;
	sim->coders[i].last_compile_start = 0;
	if (pthread_mutex_init(&sim->coders[i].c_mutex, NULL))
	{
		destroy_dongles(sim, sim->number_of_coders);
		destroy_coders(sim, i);
		free(sim->coders);
		free(sim->dongles);
		return (1);
	}
	sim->coders[i].left = &sim->dongles[i];
	sim->coders[i].right = &sim->dongles[(i + 1) % sim->number_of_coders];
	return (0);
}

int	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (init_coder(sim, i))
			return (1);
		i++;
	}
	return (0);
}

int	init_sync(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_mutex, NULL))
	{
		abort_sync(sim, 0);
		return (1);
	}
	if (pthread_mutex_init(&sim->start_mutex, NULL))
	{
		abort_sync(sim, 1);
		return (1);
	}
	if (pthread_cond_init(&sim->start_cond, NULL))
	{
		abort_sync(sim, 2);
		return (1);
	}
	if (pthread_mutex_init(&sim->stop_mutex, NULL))
	{
		abort_sync(sim, 3);
		return (1);
	}
	sim->simulation_started = 0;
	sim->stop_flag = 0;
	return (0);
}

int	init_simulation(t_sim *sim)
{
	if (alloc_simulation(sim))
		return (1);
	if (init_dongles(sim))
		return (1);
	if (init_coders(sim))
		return (1);
	if (init_sync(sim))
		return (1);
	init_heaps(sim);
	return (0);
}
