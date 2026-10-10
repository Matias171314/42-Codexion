/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 21:27:39 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 21:27:43 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	sleep_ms(t_ms duration)
{
	t_ms	start;
	t_ms	elapsed;
	t_ms	remaining;

	if (duration <= 0)
		return ;
	start = get_time_ms();
	elapsed = 0;
	while (elapsed < duration)
	{
		remaining = duration - elapsed;
		if (remaining > 2)
			usleep(1000);
		else
			usleep(200);
		elapsed = get_time_ms() - start;
	}
}
