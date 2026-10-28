/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_extra.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:44:54 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:44:55 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_up(t_heap *heap, int index)
{
	int	parent;
	int	i;

	i = index;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (higher_priority(heap->array[i], heap->array[parent]))
			swap((t_coder **)&heap->array[parent],
				(t_coder **)&heap->array[i]);
		else
			break ;
		i = parent;
	}
}

void	heap_down(t_heap *heap, int index)
{
	int	i;
	int	l_child;
	int	r_child;
	int	best;

	i = index;
	while ((i * 2) + 1 < heap->count)
	{
		l_child = i * 2 + 1;
		r_child = i * 2 + 2;
		best = l_child;
		if (r_child < heap->count
			&& higher_priority(heap->array[r_child], heap->array[l_child]))
			best = r_child;
		if (higher_priority(heap->array[best], heap->array[i]))
		{
			swap((t_coder **)&heap->array[best],
				(t_coder **)&heap->array[i]);
			i = best;
		}
		else
			break ;
	}
}
