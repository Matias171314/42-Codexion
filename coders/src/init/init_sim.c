/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:55:07 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 21:52:42 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

static int	init_start_sync(t_sim *sim)
{
	if (pthread_mutex_init(&sim->start_mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&sim->start_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->start_mutex);
		return (0);
	}
	return (1);
}

static int	init_main_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		return (0);
	}
	if (!init_start_sync(sim))
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->print_mutex);
		return (0);
	}
	return (1);
}

static void	cleanup_after_scheduler(t_sim *sim)
{
	destroy_scheduler(sim);
	destroy_main_sync(sim);
}

static void	cleanup_after_dongles(t_sim *sim)
{
	destroy_dongles(sim);
	destroy_scheduler(sim);
	destroy_main_sync(sim);
}

int	init_sim(t_sim *sim, const t_config *config)
{
	memset(sim, 0, sizeof(t_sim));
	sim->config = *config;
	if (!init_main_mutexes(sim))
		return (0);
	if (!init_scheduler(sim))
	{
		destroy_main_sync(sim);
		return (0);
	}
	if (!init_dongles(sim))
	{
		cleanup_after_scheduler(sim);
		return (0);
	}
	if (!init_coders(sim))
	{
		cleanup_after_dongles(sim);
		return (0);
	}
	return (1);
}
