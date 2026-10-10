/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:15:24 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:15:26 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	find_request(t_heap *heap, t_request *request)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->items[i] == request)
			return (i);
		i++;
	}
	return (-1);
}

static void	swap_requests(t_request **a, t_request **b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *heap, int index, t_policy policy)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_has_priority(heap->items[index],
				heap->items[parent], policy))
			break ;
		swap_requests(&heap->items[index],
			&heap->items[parent]);
		index = parent;
	}
}

static void	sift_down(t_heap *heap, int index, t_policy policy)
{
	int	left;
	int	right;
	int	best;

	while (index * 2 + 1 < heap->size)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		best = left;
		if (right < heap->size
			&& request_has_priority(heap->items[right],
				heap->items[left], policy))
			best = right;
		if (!request_has_priority(heap->items[best],
				heap->items[index], policy))
			break ;
		swap_requests(&heap->items[index],
			&heap->items[best]);
		index = best;
	}
}

int	heap_remove(t_heap *heap, t_request *request, t_policy policy)
{
	int	index;
	int	parent;

	index = find_request(heap, request);
	if (index < 0)
		return (0);
	heap->size--;
	if (index == heap->size)
		return (heap->items[index] = NULL, 1);
	heap->items[index] = heap->items[heap->size];
	heap->items[heap->size] = NULL;
	parent = (index - 1) / 2;
	if (index > 0 && request_has_priority(heap->items[index],
			heap->items[parent], policy))
		sift_up(heap, index, policy);
	else
		sift_down(heap, index, policy);
	return (1);
}
