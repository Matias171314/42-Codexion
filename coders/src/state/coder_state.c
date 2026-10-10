/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:24:06 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:24:09 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_set_compile_start(t_coder *coder, t_ms timestamp)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = timestamp;
	pthread_mutex_unlock(&coder->state_mutex);
}

void	coder_increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
}

static int	coder_reached_required(t_coder *coder, int required)
{
	int	count;

	pthread_mutex_lock(&coder->state_mutex);
	count = coder->compile_count;
	pthread_mutex_unlock(&coder->state_mutex);
	return (count >= required);
}

int	sim_all_compiles_completed(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (!coder_reached_required(&sim->coders[i],
				sim->config.number_of_compiles_required))
			return (0);
		i++;
	}
	return (1);
}
