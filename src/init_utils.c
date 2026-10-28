/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:44:41 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 19:52:10 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_heaps(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].heap.capacity = 2;
		sim->dongles[i].heap.count = 0;
		i++;
	}
}

int	alloc_simulation(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
	{
		fprintf(stderr, "[Error]: failed to allocate coders\n\n");
		return (1);
	}
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->dongles)
	{
		fprintf(stderr, "[Error]: failed to allocate dongles\n\n");
		free(sim->coders);
		return (1);
	}
	return (0);
}

void	destroy_dongles(t_sim *sim, int count)
{
	while (count > 0)
	{
		count--;
		pthread_mutex_destroy(&sim->dongles[count].d_mutex);
	}
}

int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].d_mutex, NULL))
		{
			destroy_dongles(sim, i);
			free(sim->coders);
			free(sim->dongles);
			return (1);
		}
		sim->dongles[i].available_at = 0;
		sim->dongles[i].in_process = 0;
		i++;
	}
	return (0);
}
