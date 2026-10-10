/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 21:46:53 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/09 15:38:32 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_for_start(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->start_mutex);
	sim->ready_count++;
	pthread_cond_broadcast(&sim->start_cond);
	while (!sim->started)
		pthread_cond_wait(&sim->start_cond, &sim->start_mutex);
	pthread_mutex_unlock(&sim->start_mutex);
}

static int	simulation_already_complete(t_sim *sim)
{
	if (!sim_all_compiles_completed(sim))
		return (0);
	stop_sim(sim);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_for_start(coder);
	if (simulation_already_complete(coder->sim))
		return (NULL);
	while (!sim_is_stopped(coder->sim))
	{
		if (!coder_cycle(coder))
			break ;
	}
	return (NULL);
}
