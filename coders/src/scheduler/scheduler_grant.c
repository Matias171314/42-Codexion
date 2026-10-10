/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_grant.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:16:23 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:36:25 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	requests_share_dongle(t_request *a, t_request *b)
{
	t_coder	*ca;
	t_coder	*cb;

	ca = a->coder;
	cb = b->coder;
	return (ca->left == cb->left
		|| ca->left == cb->right
		|| ca->right == cb->left
		|| ca->right == cb->right);
}

static int	has_higher_conflict(t_sim *sim, t_request *request)
{
	t_request	*other;
	int			i;

	i = 0;
	while (i < sim->scheduler.heap.size)
	{
		other = sim->scheduler.heap.items[i];
		if (other != request && other->active
			&& requests_share_dongle(other, request)
			&& request_has_priority(other, request,
				sim->scheduler.policy))
			return (1);
		i++;
	}
	return (0);
}

int	scheduler_try_grant(t_coder *coder)
{
	t_sim		*sim;
	t_request	*request;
	t_ms		now;

	sim = coder->sim;
	request = &coder->request;
	if (has_higher_conflict(sim, request))
		return (0);
	now = elapsed_ms(sim);
	if (now >= request->deadline)
		return (0);
	if (!dongles_try_take_pair(coder, now))
		return (0);
	heap_remove(&sim->scheduler.heap, request,
		sim->scheduler.policy);
	request->active = 0;
	return (1);
}

void	scheduler_release_dongles(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->scheduler.mutex);
	dongles_release_pair(coder, elapsed_ms(sim));
	pthread_mutex_unlock(&sim->scheduler.mutex);
}
