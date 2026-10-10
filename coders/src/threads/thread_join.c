/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 23:42:30 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 23:42:32 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	join_coder_threads(t_sim *sim)
{
	int	i;
	int	status;

	i = 0;
	status = 1;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
			status = 0;
		i++;
	}
	return (status);
}

int	join_monitor_thread(t_sim *sim)
{
	if (pthread_join(sim->monitor_thread, NULL) != 0)
		return (0);
	return (1);
}
