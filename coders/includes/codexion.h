/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 09:39:13 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/03 15:34:47 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>

typedef long long	t_ms;

typedef enum e_policy
{
	POLICY_FIFO,
	POLICY_EDF
}	t_policy;

typedef enum e_log_event
{
	LOG_TAKEN_DONGLE,
	LOG_COMPILING,
	LOG_DEBUGGING,
	LOG_REFACTORING,
	LOG_BURNOUT
}	t_log_event;

typedef struct s_config
{
	int			number_of_coders;
	t_ms		time_to_burnout;
	t_ms		time_to_compile;
	t_ms		time_to_debug;
	t_ms		time_to_refactor;
	int			number_of_compiles_required;
	t_ms		dongle_cooldown;
	t_policy	policy;
}	t_config;

typedef struct s_dongle
{
	int				id;
	int				in_use;
	t_ms			cooldown_until;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_request
{
	struct s_coder	*coder;
	unsigned long	arrival_order;
	t_ms			deadline;
	int				active;
}	t_request;

typedef struct s_heap
{
	t_request	**items;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_scheduler
{
	t_policy		policy;
	t_heap			heap;
	unsigned long	next_order;
	pthread_mutex_t	mutex;
}	t_scheduler;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	t_ms			last_compile_start;
	int				compile_count;
	pthread_mutex_t	state_mutex;
	t_request		request;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_config		config;
	t_ms			start_time;
	t_coder			*coders;
	t_dongle		*dongles;
	t_scheduler		scheduler;
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	start_mutex;
	pthread_cond_t	start_cond;
	int				ready_count;
	int				started;
	int				stopped;
}	t_sim;

/* Parsing */

int			parse_config(t_config *config, int argc, char **argv);
int			parse_ms(const char *str, t_ms *value);
int			parse_int_nonnegative(const char *str, int *value);
int			parse_int_positive(const char *str, int *value);
int			parse_policy(const char *str, t_policy *policy);

/* Initialization */

int			init_sim(t_sim *sim, const t_config *config);
int			init_scheduler(t_sim *sim);
int			init_dongles(t_sim *sim);
int			init_coders(t_sim *sim);

/* Cleanup */

void		destroy_sim(t_sim *sim);
void		destroy_scheduler(t_sim *sim);
void		destroy_dongles(t_sim *sim);
void		destroy_coders(t_sim *sim);
void		destroy_main_sync(t_sim *sim);

/* Time */

t_ms		get_time_ms(void);
t_ms		elapsed_ms(t_sim *sim);
t_ms		deadline_ms(t_ms last_compile, t_ms burnout);
void		start_sim_clock(t_sim *sim);
void		sleep_ms(t_ms duration);
int			sim_sleep(t_sim *sim, t_ms duration);

/* Logging */

int			log_event(t_coder *coder, t_log_event event);

/* Simulation state */

int			sim_is_stopped(t_sim *sim);
void		stop_sim(t_sim *sim);
int			sim_all_compiles_completed(t_sim *sim);

/* Coder state */

void		coder_increment_compile_count(t_coder *coder);

/* Dongles */

int			dongle_is_available(t_dongle *dongle, t_ms now);
int			dongle_try_take(t_dongle *dongle, t_ms now);
void		dongle_release(t_dongle *dongle, t_ms now,
				t_ms cooldown);
int			dongles_try_take_pair(t_coder *coder, t_ms now);
void		dongles_release_pair(t_coder *coder, t_ms now);

/* Heap */

int			request_has_priority(t_request *a,
				t_request *b, t_policy policy);
int			heap_push(t_heap *heap, t_request *request,
				t_policy policy);
t_request	*heap_peek(t_heap *heap);
t_request	*heap_pop(t_heap *heap, t_policy policy);
int			heap_remove(t_heap *heap, t_request *request,
				t_policy policy);

/* Scheduler */

int			scheduler_request_dongles(t_coder *coder);
int			scheduler_try_grant(t_coder *coder);
void		scheduler_release_dongles(t_coder *coder);
void		scheduler_wait_event(t_sim *sim);

/* Threads */

void		*coder_routine(void *arg);
int			coder_cycle(t_coder *coder);
int			create_coder_threads(t_sim *sim);
int			create_monitor_thread(t_sim *sim);
void		start_threads(t_sim *sim);
int			join_coder_threads(t_sim *sim);
int			join_monitor_thread(t_sim *sim);

/* Monitor */

void		*monitor_routine(void *arg);

#endif