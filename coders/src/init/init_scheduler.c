/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scheduler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:53:16 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:37:50 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

int	init_scheduler(t_sim *sim)
{
	t_scheduler	*scheduler;

	scheduler = &sim->scheduler;
	scheduler->policy = sim->config.policy;
	scheduler->heap.size = 0;
	scheduler->heap.capacity = sim->config.number_of_coders;
	scheduler->next_order = 0;
	scheduler->heap.items = malloc(sizeof(t_request *)
			* scheduler->heap.capacity);
	if (!scheduler->heap.items)
		return (0);
	if (pthread_mutex_init(&scheduler->mutex, NULL) != 0)
	{
		free(scheduler->heap.items);
		scheduler->heap.items = NULL;
		return (0);
	}
	return (1);
}
