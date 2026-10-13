/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_pair.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:15:43 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:15:46 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	order_pair(t_coder *coder, t_dongle **first,
		t_dongle **second)
{
	if (coder->left->id < coder->right->id)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

static void	lock_pair(t_dongle *first, t_dongle *second)
{
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
}

static void	unlock_pair(t_dongle *first, t_dongle *second)
{
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}

int	dongles_try_take_pair(t_coder *coder, t_ms now)
{
	t_dongle	*first;
	t_dongle	*second;
	int			success;

	if (coder->left == coder->right)
		return (0);
	order_pair(coder, &first, &second);
	lock_pair(first, second);
	success = 0;
	if (!first->in_use && !second->in_use
		&& now >= first->cooldown_until
		&& now >= second->cooldown_until)
	{
		first->in_use = 1;
		second->in_use = 1;
		success = 1;
	}
	unlock_pair(first, second);
	return (success);
}

void	dongles_release_pair(t_coder *coder, t_ms now)
{
	t_dongle	*first;
	t_dongle	*second;
	t_ms		cooldown;

	if (coder->left == coder->right)
		return ;
	cooldown = coder->sim->config.dongle_cooldown;
	order_pair(coder, &first, &second);
	lock_pair(first, second);
	first->in_use = 0;
	second->in_use = 0;
	first->cooldown_until = deadline_ms(now, cooldown);
	second->cooldown_until = deadline_ms(now, cooldown);
	unlock_pair(first, second);
}
