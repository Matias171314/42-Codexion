/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:02:44 by mvasquez          #+#    #+#             */
/*   Updated: 2026/08/17 17:02:04 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_sim(t_sim *sim)
{
	sim->flag_stop = 0;
	sim->start_time = 0;
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->flag_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		return (0);
	}
	return (1);
}

void	ft_destroy_dongles(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->dongles[i]);
		i++;
	}
	free(sim->dongles);
}

int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(pthread_mutex_t) * sim->number_of_coders);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i], NULL) != 0)
		{
			ft_destroy_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_coders(t_sim *sim, t_coder **coders)
{
	int	i;

	*coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!*coders)
		return (0);
	i = 0;
	while (i < sim->number_of_coders)
	{
		(*coders)[i].id = i + 1;
		(*coders)[i].last_compile = 0;
		(*coders)[i].sim_data = sim;
		(*coders)[i].left_dongle = &sim->dongles[i];
		(*coders)[i].right_dongle = &sim->dongles[(i + 1)
			% sim->number_of_coders];
		i++;
	}
	return (1);
}
