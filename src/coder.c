/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:45:23 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:45:24 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	prepare_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->start_mutex);
	while (!coder->data->simulation_started)
		pthread_cond_wait(&coder->data->start_cond, &coder->data->start_mutex);
	pthread_mutex_unlock(&coder->data->start_mutex);
	if (coder->id % 2 == 0)
		wait_ms(((coder->data->time_to_compile
					+ coder->data->dongle_cooldown) / 2), coder->data);
}

void	run_coder(t_coder *coder)
{
	while (should_stop(coder->data) == 0)
	{
		queue_dongles(coder);
		if (should_stop(coder->data))
			break ;
		compile(coder);
		release_dongles(coder);
		pthread_mutex_lock(&coder->c_mutex);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->c_mutex);
		debug_and_refactor(coder);
	}
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	prepare_coder(coder);
	run_coder(coder);
	return (NULL);
}
