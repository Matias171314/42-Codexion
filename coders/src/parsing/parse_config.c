/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:43:32 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/04 12:39:31 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

int	parse_config(t_config *config, int argc, char **argv)
{
	if (argc != 9)
		return (0);
	memset(config, 0, sizeof(t_config));
	if (!parse_int_positive(argv[1], &config->number_of_coders))
		return (0);
	if (!parse_ms(argv[2], &config->time_to_burnout))
		return (0);
	if (!parse_ms(argv[3], &config->time_to_compile))
		return (0);
	if (!parse_ms(argv[4], &config->time_to_debug))
		return (0);
	if (!parse_ms(argv[5], &config->time_to_refactor))
		return (0);
	if (!parse_int_nonnegative(argv[6],
			&config->number_of_compiles_required))
		return (0);
	if (!parse_ms(argv[7], &config->dongle_cooldown))
		return (0);
	if (!parse_policy(argv[8], &config->policy))
		return (0);
	return (1);
}
