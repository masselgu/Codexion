/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:45:17 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:48:15 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_sim	t_sim;

typedef struct s_heap
{
	void	*array[2];
	int		count;
	int		capacity;
}	t_heap;

typedef struct s_dongle
{
	long			available_at;
	int				in_process;
	pthread_mutex_t	d_mutex;
	t_heap			heap;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	long			last_compile_start;
	int				compile_count;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	t_sim			*data;
	pthread_mutex_t	c_mutex;
	size_t			request_time;
	size_t			deadline;
}	t_coder;

typedef struct s_sim
{
	long			start_time;
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				stop_flag;
	int				scheduler_mode;
	int				simulation_started;

	pthread_mutex_t	print_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	stop_mutex;

	pthread_mutex_t	start_mutex;
	pthread_cond_t	start_cond;

}	t_sim;

int		valid_number(char *str);
int		check_sign(char *str, int i);
int		parsing(char **av, t_sim *sim);
long	ft_atoi(char *str);
int		skip_sign(char *str, int i);
void	swap(t_coder **a, t_coder **b);
void	push(t_heap *heap, t_coder *coder);
t_coder	*pop(t_heap *heap);
void	heap_up(t_heap *heap, int index);
void	heap_down(t_heap *heap, int index);
void	wait_ms(size_t duration, t_sim *sim);
size_t	get_time(void);
void	queue_dongles(t_coder *coder);
void	compile(t_coder *coder);
void	release_dongles(t_coder *coder);
void	debug_and_refactor(t_coder *coder);
void	log_action(t_coder *coder, char *status);
int		should_stop(t_sim *sim);
int		check_burnout(t_sim *sim);
int		all_coders_done(t_sim *sim);
void	*monitor_thread(void *arg);
int		higher_priority(t_coder *a, t_coder *b);
int		request_dongles(t_coder *coder);
int		dongle_available(t_dongle *dongle, t_coder *coder);
void	free_simulation(t_sim *sim);
void	destroy_sync(t_sim *sim);
void	wait_for_dongles(t_coder *coder);
int		validate_value(char **av, int index);
int		validate_number(char **av, int index);
int		validate_args(char **av);
int		abort_simulation(t_sim *sim, int count);
int		setup_simulation(t_sim *sim, pthread_t *monitor);
int		start_simulation(t_sim *sim);
void	prepare_coder(t_coder *coder);
void	run_coder(t_coder *coder);
void	*coder_routine(void *arg);
void	init_compile_times(t_sim *sim);
void	destroy_coders(t_sim *sim, int count);
int		init_coder(t_sim *sim, int i);
int		init_coders(t_sim *sim);
int		init_sync(t_sim *sim);
int		init_simulation(t_sim *sim);
void	init_heaps(t_sim *sim);
int		alloc_simulation(t_sim *sim);
void	destroy_dongles(t_sim *sim, int count);
int		init_dongles(t_sim *sim);
void	abort_sync(t_sim *sim, int count);
void	cleanup_sync(t_sim *sim, int count);
int		try_take_dongles(t_dongle *first, t_dongle *second, t_coder *coder);

#endif
