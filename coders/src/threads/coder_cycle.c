/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:24:47 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:24:50 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	release_dongles(t_coder *coder)
{
	scheduler_release_dongles(coder);
}

static int	begin_compile(t_coder *coder)
{
	t_sim	*sim;
	t_ms	start;

	sim = coder->sim;
	if (!scheduler_request_dongles(coder))
		return (0);
	log_event(coder, LOG_TAKEN_DONGLE);
	log_event(coder, LOG_TAKEN_DONGLE);
	if (sim_is_stopped(sim))
		return (release_dongles(coder), 0);
	start = elapsed_ms(sim);
	coder_set_compile_start(coder, start);
	log_event(coder, LOG_COMPILING);
	if (sim_is_stopped(sim))
		return (release_dongles(coder), 0);
	return (1);
}

static int	finish_compile(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (!sim_sleep(sim, sim->config.time_to_compile))
		return (release_dongles(coder), 0);
	if (sim_is_stopped(sim))
		return (release_dongles(coder), 0);
	coder_increment_compile_count(coder);
	if (sim_all_compiles_completed(sim))
	{
		stop_sim(sim);
		release_dongles(coder);
		return (0);
	}
	release_dongles(coder);
	return (1);
}

static int	debug_and_refactor(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	log_event(coder, LOG_DEBUGGING);
	if (!sim_sleep(sim, sim->config.time_to_debug))
		return (0);
	log_event(coder, LOG_REFACTORING);
	if (!sim_sleep(sim, sim->config.time_to_refactor))
		return (0);
	return (!sim_is_stopped(sim));
}

int	coder_cycle(t_coder *coder)
{
	if (!begin_compile(coder))
		return (0);
	if (!finish_compile(coder))
		return (0);
	return (debug_and_refactor(coder));
}
