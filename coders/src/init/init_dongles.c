/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:54:20 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 18:54:22 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

static void	destroy_initialized_dongles(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

int	init_dongles(t_sim *sim)
{
	int	i;
	int	count;

	count = sim->config.number_of_coders;
	sim->dongles = malloc(sizeof(t_dongle) * count);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < count)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].in_use = 0;
		sim->dongles[i].cooldown_until = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			destroy_initialized_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}
