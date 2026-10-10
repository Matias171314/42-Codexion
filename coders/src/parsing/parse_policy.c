/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_policy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:43:20 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 18:43:24 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

int	parse_policy(const char *str, t_policy *policy)
{
	if (strcmp(str, "fifo") == 0)
	{
		*policy = POLICY_FIFO;
		return (1);
	}
	if (strcmp(str, "edf") == 0)
	{
		*policy = POLICY_EDF;
		return (1);
	}
	return (0);
}
