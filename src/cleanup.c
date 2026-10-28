/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:45:28 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:46:48 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_simulation(t_sim *sim)
{
	if (sim->coders)
		free(sim->coders);
	if (sim->dongles)
		free(sim->dongles);
}
