/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 21:58:27 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:25:18 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static void	wait_for_start(t_sim *sim)
{
	pthread_mutex_lock(&sim->start_mutex);
	while (!sim->started)
		pthread_cond_wait(&sim->start_cond, &sim->start_mutex);
	pthread_mutex_unlock(&sim->start_mutex);
}

static t_ms	get_coder_deadline(t_coder *coder)
{
	t_ms	last_compile;

	pthread_mutex_lock(&coder->state_mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	return (deadline_ms(last_compile,
			coder->sim->config.time_to_burnout));
}

static int	check_burnout(t_sim *sim)
{
	int		i;
	t_ms	now;
	t_ms	deadline;

	now = elapsed_ms(sim);
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		deadline = get_coder_deadline(&sim->coders[i]);
		if (now >= deadline
			&& log_event(&sim->coders[i], LOG_BURNOUT))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	wait_for_start(sim);
	while (!sim_is_stopped(sim))
	{
		if (sim_all_compiles_completed(sim))
		{
			stop_sim(sim);
			break ;
		}
		if (check_burnout(sim))
			break ;
		usleep(500);
	}
	return (NULL);
}
