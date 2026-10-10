/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 21:47:18 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/09 13:12:53 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	join_created(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

static void	abort_before_start(t_sim *sim, int coder_count)
{
	stop_sim(sim);
	pthread_mutex_lock(&sim->start_mutex);
	sim->started = 1;
	pthread_cond_broadcast(&sim->start_cond);
	pthread_mutex_unlock(&sim->start_mutex);
	join_created(sim, coder_count);
}

int	create_coder_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
		{
			abort_before_start(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	create_monitor_thread(t_sim *sim)
{
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
	{
		abort_before_start(sim, sim->config.number_of_coders);
		return (0);
	}
	return (1);
}

void	start_threads(t_sim *sim)
{
	pthread_mutex_lock(&sim->start_mutex);
	while (sim->ready_count < sim->config.number_of_coders)
		pthread_cond_wait(&sim->start_cond, &sim->start_mutex);
	start_sim_clock(sim);
	sim->started = 1;
	pthread_cond_broadcast(&sim->start_cond);
	pthread_mutex_unlock(&sim->start_mutex);
}
