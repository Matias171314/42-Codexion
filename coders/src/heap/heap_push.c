/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_push.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:14:55 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:14:58 by mvasquez         ###   ########.fr       */
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

int	heap_push(t_heap *heap, t_request *request, t_policy policy)
{
	int	index;

	if (heap->size >= heap->capacity)
		return (0);
	index = heap->size;
	heap->items[index] = request;
	heap->size++;
	sift_up(heap, index, policy);
	return (1);
}
