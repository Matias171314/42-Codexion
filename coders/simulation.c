/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 20:16:12 by mvasquez          #+#    #+#             */
/*   Updated: 2026/08/19 16:33:10 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_compile_step(t_coder *coder)
{
	pthread_mutex_lock(coder->left_dongle);
	pthread_mutex_lock(coder->right_dongle);
	pthread_mutex_lock(&coder->sim_data->print_mutex);
	printf("%lld %d is compiling\n", coder->last_compile, coder->id);
	pthread_mutex_unlock(&coder->sim_data->print_mutex);
	usleep(coder->sim_data->time_to_compile * 1000);
	pthread_mutex_unlock(coder->right_dongle);
	pthread_mutex_unlock(coder->left_dongle);
}

void	ft_debug_step(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim_data->print_mutex);
	printf("%lld %d is debuging\n", coder->last_compile, coder->id);
	pthread_mutex_unlock(&coder->sim_data->print_mutex);
	usleep(coder->sim_data->time_to_refactor * 1000);
}

void	ft_refactor_step(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim_data->print_mutex);
	printf("%lld %d is refactoring\n", coder->last_compile, coder->id);
	pthread_mutex_unlock(&coder->sim_data->print_mutex);
}

int	ft_check_simulation(t_coder *coder)
{
	int	status;

	pthread_mutex_lock(&coder->sim_data->flag_mutex);
	status = coder->sim_data->flag_stop;
	pthread_mutex_unlock(&coder->sim_data->flag_mutex);
	return (status);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep((coder->sim_data->time_to_compile * 1000) / 2);
	while (!ft_check_simulation(coder))
	{
		ft_compile_step(coder);
		ft_debug_step(coder);
		ft_refactor_step(coder);
	}
	return (NULL);
}
