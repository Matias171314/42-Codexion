/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_compare.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:06:28 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/03 00:42:58 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_compare.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 00:00:00 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/03 00:00:00 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fifo_priority(t_request *a, t_request *b)
{
	return (a->arrival_order < b->arrival_order);
}

static int	edf_priority(t_request *a, t_request *b)
{
	if (a->deadline < b->deadline)
		return (1);
	return (0);
}

int	request_has_priority(t_request *a, t_request *b,
		t_policy policy)
{
	if (policy == POLICY_FIFO)
		return (fifo_priority(a, b));
	return (edf_priority(a, b));
}
