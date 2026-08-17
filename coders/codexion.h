/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 09:39:13 by mvasquez          #+#    #+#             */
/*   Updated: 2026/08/17 20:10:22 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct	s_sim
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				num_compiles_req;
	int				dongle_cooldown;
	int				scheduler;
	long long		start_time;
	int				flag_stop;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	flag_mutex;
	pthread_mutex_t	*dongles;
}				t_sim;

typedef struct	s_coder
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
	long long		last_compile;
	t_sim			*sim_data;
}				t_coder;

#endif