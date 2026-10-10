/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 11:33:23 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/09 13:08:47 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

static int	start_simulation(t_sim *sim)
{
	if (!create_coder_threads(sim))
	{
		fprintf(stderr, "Error: coder thread creation failed\n");
		return (0);
	}
	if (!create_monitor_thread(sim))
	{
		fprintf(stderr, "Error: monitor creation failed\n");
		return (0);
	}
	start_threads(sim);
	return (1);
}

static void	wait_simulation(t_sim *sim)
{
	if (!join_monitor_thread(sim))
		fprintf(stderr, "Error: monitor join failed\n");
	if (!join_coder_threads(sim))
		fprintf(stderr, "Error: coder thread join failed\n");
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_sim		sim;

	if (!parse_config(&config, argc, argv))
		return (fprintf(stderr, "Error: invalid arguments\n"), 1);
	if (!init_sim(&sim, &config))
		return (fprintf(stderr, "Error: initialization failed\n"), 1);
	if (!start_simulation(&sim))
	{
		destroy_sim(&sim);
		return (1);
	}
	wait_simulation(&sim);
	destroy_sim(&sim);
	return (0);
}
