/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 18:02:50 by mvasquez          #+#    #+#             */
/*   Updated: 2026/08/17 20:10:19 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_is_valid_int(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_safe_atoi(char *str)
{
	int	result;

	if (!ft_is_valid_int(str))
		return (-1);
	result = atoi(str);
	return (result);
}

int	parse_arguments(char **argv, t_sim *sim)
{
	sim->number_of_coders = ft_safe_atoi(argv[1]);
	sim->time_to_burnout = ft_safe_atoi(argv[2]);
	sim->time_to_compile = ft_safe_atoi(argv[3]);
	sim->time_to_debug = ft_safe_atoi(argv[4]);
	sim->time_to_refactor = ft_safe_atoi(argv[5]);
	sim->num_compiles_req = ft_safe_atoi(argv[6]);
	sim->dongle_cooldown = ft_safe_atoi(argv[7]);
	if (sim->number_of_coders <= 0 || sim->time_to_burnout < 0
		|| sim->time_to_compile < 0 || sim->time_to_debug < 0
		|| sim->time_to_refactor < 0 || sim->num_compiles_req < 0
		|| sim->dongle_cooldown < 0)
		return (0);
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler = 0;
	else if (strcmp(argv[8], "edf") == 0)
		sim->scheduler = 1;
	else
		return (0);
	return (1);
}
