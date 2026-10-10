/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_sleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:23:51 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:23:54 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

int	sim_sleep(t_sim *sim, t_ms duration)
{
	t_ms	start;
	t_ms	elapsed;
	t_ms	remaining;

	if (duration <= 0)
		return (!sim_is_stopped(sim));
	start = get_time_ms();
	elapsed = 0;
	while (elapsed < duration)
	{
		if (sim_is_stopped(sim))
			return (0);
		remaining = duration - elapsed;
		if (remaining > 2)
			usleep(500);
		else
			usleep(100);
		elapsed = get_time_ms() - start;
	}
	return (!sim_is_stopped(sim));
}
