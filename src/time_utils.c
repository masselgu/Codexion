/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:42:52 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:42:53 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	wait_ms(size_t duration, t_sim *sim)
{
	size_t	start_time;

	start_time = get_time();
	while (!should_stop(sim))
	{
		if (get_time() - start_time >= duration)
			break ;
		usleep(500);
	}
}
