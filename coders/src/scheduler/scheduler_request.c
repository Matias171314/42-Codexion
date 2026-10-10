/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_request.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:16:38 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:36:32 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_ms	get_request_deadline(t_coder *coder)
{
	t_ms	last_compile;

	pthread_mutex_lock(&coder->state_mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	return (deadline_ms(last_compile,
			coder->sim->config.time_to_burnout));
}

static int	prepare_request(t_coder *coder, t_ms deadline)
{
	t_sim		*sim;
	t_request	*request;

	sim = coder->sim;
	request = &coder->request;
	if (request->active)
		return (0);
	request->arrival_order = sim->scheduler.next_order++;
	request->deadline = deadline;
	request->active = 1;
	if (!heap_push(&sim->scheduler.heap, request,
			sim->scheduler.policy))
	{
		request->active = 0;
		return (0);
	}
	return (1);
}

static void	cancel_request(t_coder *coder)
{
	t_sim		*sim;
	t_request	*request;

	sim = coder->sim;
	request = &coder->request;
	if (!request->active)
		return ;
	heap_remove(&sim->scheduler.heap, request,
		sim->scheduler.policy);
	request->active = 0;
}

static int	wait_for_grant(t_coder *coder)
{
	while (!sim_is_stopped(coder->sim))
	{
		if (scheduler_try_grant(coder))
			return (1);
		scheduler_wait_event(coder->sim);
	}
	return (0);
}

int	scheduler_request_dongles(t_coder *coder)
{
	t_sim	*sim;
	t_ms	deadline;
	int		granted;

	if (coder->left == coder->right)
		return (0);
	sim = coder->sim;
	deadline = get_request_deadline(coder);
	pthread_mutex_lock(&sim->scheduler.mutex);
	if (sim_is_stopped(sim))
	{
		pthread_mutex_unlock(&sim->scheduler.mutex);
		return (0);
	}
	if (!prepare_request(coder, deadline))
	{
		pthread_mutex_unlock(&sim->scheduler.mutex);
		return (0);
	}
	granted = wait_for_grant(coder);
	if (!granted)
		cancel_request(coder);
	pthread_mutex_unlock(&sim->scheduler.mutex);
	return (granted);
}
