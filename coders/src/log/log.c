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

static int	prepare_event(t_coder *coder, t_log_event event, t_ms now)
{
	t_ms	deadline;
	int		valid;

	if (event != LOG_COMPILING && event != LOG_BURNOUT)
		return (1);
	pthread_mutex_lock(&coder->state_mutex);
	deadline = deadline_ms(coder->last_compile_start,
			coder->sim->config.time_to_burnout);
	valid = (now < deadline);
	if (event == LOG_BURNOUT)
		valid = !valid;
	if (valid && event == LOG_COMPILING)
		coder->last_compile_start = now;
	if (valid && event == LOG_BURNOUT)
		coder->sim->stopped = 1;
	pthread_mutex_unlock(&coder->state_mutex);
	return (valid);
}

int	log_event(t_coder *coder, t_log_event event)
{
	t_sim		*sim;
	const char	*message;
	t_ms		timestamp;

	sim = coder->sim;
	message = event_message(event);
	if (!message)
		return (0);
	pthread_mutex_lock(&sim->stop_mutex);
	timestamp = elapsed_ms(sim);
	if (sim->stopped || !prepare_event(coder, event, timestamp))
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return (0);
	}
	pthread_mutex_lock(&sim->print_mutex);
	printf("%lld %d %s\n", timestamp, coder->id, message);
	pthread_mutex_unlock(&sim->print_mutex);
	pthread_mutex_unlock(&sim->stop_mutex);
	return (1);
}
