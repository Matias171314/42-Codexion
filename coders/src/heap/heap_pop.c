/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_pop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:15:09 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:15:11 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_requests(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	best_child(t_heap *heap, int index, t_policy policy)
{
	int	left;
	int	right;
	int	best;

	left = index * 2 + 1;
	right = index * 2 + 2;
	best = left;
	if (right < heap->size
		&& request_has_priority(heap->items[right],
			heap->items[left], policy))
		best = right;
	return (best);
}

static void	sift_down(t_heap *heap, int index, t_policy policy)
{
	int	best;

	while (index * 2 + 1 < heap->size)
	{
		best = best_child(heap, index, policy);
		if (!request_has_priority(heap->items[best],
				heap->items[index], policy))
			break ;
		swap_requests(&heap->items[index],
			&heap->items[best]);
		index = best;
	}
}

t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->items[0]);
}

t_request	*heap_pop(t_heap *heap, t_policy policy)
{
	t_request	*top;

	if (heap->size == 0)
		return (NULL);
	top = heap->items[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->items[0] = heap->items[heap->size];
		sift_down(heap, 0, policy);
	}
	heap->items[heap->size] = NULL;
	return (top);
}
