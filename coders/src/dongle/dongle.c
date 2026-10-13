/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:02:39 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:17:08 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_is_available(t_dongle *dongle, t_ms now)
{
	int	available;

	pthread_mutex_lock(&dongle->mutex);
	available = (!dongle->in_use
			&& now >= dongle->cooldown_until);
	pthread_mutex_unlock(&dongle->mutex);
	return (available);
}

int	dongle_try_take(t_dongle *dongle, t_ms now)
{
	int	success;

	success = 0;
	pthread_mutex_lock(&dongle->mutex);
	if (!dongle->in_use && now >= dongle->cooldown_until)
	{
		dongle->in_use = 1;
		success = 1;
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (success);
}

void	dongle_release(t_dongle *dongle, t_ms now,
		t_ms cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->cooldown_until = deadline_ms(now, cooldown);
	pthread_mutex_unlock(&dongle->mutex);
}
