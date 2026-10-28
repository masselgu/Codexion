/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:42:40 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:42:42 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_action(t_coder *coder, char *status)
{
	size_t	timestamp;

	pthread_mutex_lock(&coder->data->print_mutex);
	if (should_stop(coder->data) == 0)
	{
		timestamp = get_time() - coder->data->start_time;
		printf("%ld %d %s\n", timestamp, coder->id, status);
	}
	pthread_mutex_unlock(&coder->data->print_mutex);
}

int	should_stop(t_sim *sim)
{
	int	status;

	pthread_mutex_lock(&sim->stop_mutex);
	status = sim->stop_flag;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (status);
}
