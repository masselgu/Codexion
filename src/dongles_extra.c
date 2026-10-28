/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:45:05 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:45:06 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_available(t_dongle *dongle, t_coder *coder)
{
	if ((t_coder *)dongle->heap.array[0] != coder)
		return (1);
	if (dongle->available_at > (long)get_time())
		return (1);
	if (dongle->in_process == 1)
		return (1);
	return (0);
}

void	wait_for_dongles(t_coder *coder)
{
	while (should_stop(coder->data) == 0)
	{
		if (request_dongles(coder) == 0)
		{
			log_action(coder, "has taken a dongle");
			log_action(coder, "has taken a dongle");
			break ;
		}
		usleep(100);
	}
}

int	try_take_dongles(t_dongle *first, t_dongle *second, t_coder *coder)
{
	pthread_mutex_lock(&first->d_mutex);
	pthread_mutex_lock(&second->d_mutex);
	if (dongle_available(first, coder) == 0
		&& dongle_available(second, coder) == 0)
	{
		first->in_process = 1;
		second->in_process = 1;
		pop(&first->heap);
		pop(&second->heap);
		pthread_mutex_unlock(&second->d_mutex);
		pthread_mutex_unlock(&first->d_mutex);
		return (0);
	}
	pthread_mutex_unlock(&second->d_mutex);
	pthread_mutex_unlock(&first->d_mutex);
	return (1);
}
