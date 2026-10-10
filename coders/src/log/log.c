/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 21:28:10 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:27:21 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

static const char	*event_message(t_log_event event)
{
	if (event == LOG_TAKEN_DONGLE)
		return ("has taken a dongle");
	if (event == LOG_COMPILING)
		return ("is compiling");
	if (event == LOG_DEBUGGING)
		return ("is debugging");
	if (event == LOG_REFACTORING)
		return ("is refactoring");
	if (event == LOG_BURNOUT)
		return ("burned out");
	return (NULL);
}

void	log_event(t_coder *coder, t_log_event event)
{
	t_sim		*sim;
	const char	*message;
	t_ms		timestamp;

	sim = coder->sim;
	message = event_message(event);
	if (!message)
		return ;
	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->stopped)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return ;
	}
	if (event == LOG_BURNOUT)
		sim->stopped = 1;
	timestamp = elapsed_ms(sim);
	pthread_mutex_lock(&sim->print_mutex);
	printf("%lld %d %s\n", timestamp, coder->id, message);
	pthread_mutex_unlock(&sim->print_mutex);
	pthread_mutex_unlock(&sim->stop_mutex);
}
