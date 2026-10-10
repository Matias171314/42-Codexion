/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_sim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:54:45 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:37:25 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	destroy_coders(t_sim *sim)
{
	int	i;

	if (!sim->coders)
		return ;
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].state_mutex);
		i++;
	}
	free(sim->coders);
	sim->coders = NULL;
}

void	destroy_dongles(t_sim *sim)
{
	int	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

void	destroy_scheduler(t_sim *sim)
{
	pthread_mutex_destroy(&sim->scheduler.mutex);
	free(sim->scheduler.heap.items);
	sim->scheduler.heap.items = NULL;
}

void	destroy_main_sync(t_sim *sim)
{
	pthread_cond_destroy(&sim->start_cond);
	pthread_mutex_destroy(&sim->start_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
}

void	destroy_sim(t_sim *sim)
{
	destroy_coders(sim);
	destroy_dongles(sim);
	destroy_scheduler(sim);
	destroy_main_sync(sim);
}
