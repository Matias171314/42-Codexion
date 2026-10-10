/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_wait.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 22:16:05 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 22:36:55 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	scheduler_wait_event(t_sim *sim)
{
	pthread_mutex_unlock(&sim->scheduler.mutex);
	usleep(500);
	pthread_mutex_lock(&sim->scheduler.mutex);
}
