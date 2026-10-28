/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:43:22 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 19:54:47 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	abort_simulation(t_sim *sim, int count)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	pthread_mutex_lock(&sim->start_mutex);
	sim->simulation_started = 1;
	pthread_cond_broadcast(&sim->start_cond);
	pthread_mutex_unlock(&sim->start_mutex);
	while (count > 0)
	{
		count--;
		pthread_join(sim->coders[count].thread, NULL);
	}
	return (1);
}

int	setup_simulation(t_sim *sim, pthread_t *monitor)
{
	int	i;
	int	ret;

	i = 0;
	while (i < sim->number_of_coders)
	{
		ret = pthread_create(&sim->coders[i].thread, NULL,
				&coder_routine, &sim->coders[i]);
		if (ret != 0)
		{
			fprintf(stderr, "[Error]: While Creating THREAD :%d \n", i);
			return (abort_simulation(sim, i));
		}
		i++;
	}
	if (pthread_create(monitor, NULL, &monitor_thread, sim) != 0)
	{
		fprintf(stderr, "[Error]: While Creating MONITOR_THREAD ===\n");
		return (abort_simulation(sim, sim->number_of_coders));
	}
	sim->start_time = get_time();
	init_compile_times(sim);
	return (0);
}

int	start_simulation(t_sim *sim)
{
	pthread_t	monitor;
	int			i;

	if (setup_simulation(sim, &monitor))
		return (1);
	pthread_mutex_lock(&sim->start_mutex);
	sim->simulation_started = 1;
	pthread_cond_broadcast(&sim->start_cond);
	pthread_mutex_unlock(&sim->start_mutex);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	return (0);
}

void	init_compile_times(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].last_compile_start = sim->start_time;
		i++;
	}
}
