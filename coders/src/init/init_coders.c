/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:53:44 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 18:55:49 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

static void	init_coder_data(t_sim *sim, t_coder *coder, int index)
{
	int	count;

	count = sim->config.number_of_coders;
	coder->id = index + 1;
	coder->left = &sim->dongles[index];
	coder->right = &sim->dongles[(index + 1) % count];
	coder->last_compile_start = 0;
	coder->compile_count = 0;
	coder->request.coder = coder;
	coder->request.arrival_order = 0;
	coder->request.deadline = 0;
	coder->request.active = 0;
	coder->sim = sim;
}

static void	destroy_initialized_coders(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->coders[i].state_mutex);
		i++;
	}
	free(sim->coders);
	sim->coders = NULL;
}

int	init_coders(t_sim *sim)
{
	int	i;
	int	count;

	count = sim->config.number_of_coders;
	sim->coders = malloc(sizeof(t_coder) * count);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < count)
	{
		init_coder_data(sim, &sim->coders[i], i);
		if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
		{
			destroy_initialized_coders(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}
