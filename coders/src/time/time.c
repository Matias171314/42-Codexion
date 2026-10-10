/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 21:27:16 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:23:42 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <limits.h>
#include <sys/time.h>

t_ms	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_ms)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

t_ms	elapsed_ms(t_sim *sim)
{
	return (get_time_ms() - sim->start_time);
}

t_ms	deadline_ms(t_ms last_compile, t_ms burnout)
{
	if (burnout > LLONG_MAX - last_compile)
		return (LLONG_MAX);
	return (last_compile + burnout);
}

void	start_sim_clock(t_sim *sim)
{
	sim->start_time = get_time_ms();
}
