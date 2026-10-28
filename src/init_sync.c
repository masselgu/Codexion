/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sync.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:44:48 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:44:49 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_sync(t_sim *sim, int count)
{
	if (count > 2)
		pthread_cond_destroy(&sim->start_cond);
	if (count > 1)
		pthread_mutex_destroy(&sim->start_mutex);
	if (count > 0)
		pthread_mutex_destroy(&sim->print_mutex);
	destroy_coders(sim, sim->number_of_coders);
	destroy_dongles(sim, sim->number_of_coders);
	free(sim->coders);
	free(sim->dongles);
}

void	abort_sync(t_sim *sim, int count)
{
	cleanup_sync(sim, count);
	destroy_coders(sim, sim->number_of_coders);
	destroy_dongles(sim, sim->number_of_coders);
	free(sim->coders);
	free(sim->dongles);
}
