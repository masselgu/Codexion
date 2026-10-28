/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:44:02 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:44:03 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_sim *sim)
{
	int		i;
	long	time_since;
	long	burn;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].c_mutex);
		time_since = get_time() - sim->coders[i].last_compile_start;
		if (time_since >= sim->time_to_burnout)
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop_flag = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			pthread_mutex_lock(&sim->print_mutex);
			burn = get_time() - sim->start_time;
			printf("%ld %d burned out\n", burn, sim->coders[i].id);
			pthread_mutex_unlock(&sim->print_mutex);
			pthread_mutex_unlock(&sim->coders[i].c_mutex);
			return (1);
		}
		pthread_mutex_unlock(&sim->coders[i].c_mutex);
		i++;
	}
	return (0);
}

int	all_coders_done(t_sim *sim)
{
	int	i;
	int	finished_coders;

	i = 0;
	finished_coders = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].c_mutex);
		if (sim->coders[i].compile_count >= sim->number_of_compiles_required)
			finished_coders++;
		pthread_mutex_unlock(&sim->coders[i].c_mutex);
		i++;
	}
	if (finished_coders == sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop_flag = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_thread(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	pthread_mutex_lock(&sim->start_mutex);
	while (!sim->simulation_started)
	{
		pthread_cond_wait(&sim->start_cond, &sim->start_mutex);
	}
	pthread_mutex_unlock(&sim->start_mutex);
	while (1)
	{
		if (check_burnout(sim) == 1)
			return (NULL);
		if (all_coders_done(sim) == 1)
			return (NULL);
		usleep(100);
	}
}
