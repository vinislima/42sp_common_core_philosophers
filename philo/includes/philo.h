/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:50:46 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/18 22:34:39 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_simulation t_simulation;

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_lock;
	t_simulation	*simulation;
	pthread_t		thread;
}	t_philosopher;

typedef struct s_simulation
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals_required;
	int				simulation_end;
	long			start_time;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	sim_lock;
	pthread_t		monitor_thread;
}	t_simulation;

int		init_simulation(t_simulation *sim, int argc, char **argv);
int		start_simulation(t_simulation *sim);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
void	cleanup_simulation(t_simulation *sim);
long	get_time(void);
void	safe_print(t_simulation *sim, int id, char *message);
void	smart_sleep(long duration);
int		check_simulation_end(t_simulation *sim);
void	acquire_forks(t_philosopher *philo);
void	release_forks(t_philosopher *philo);

#endif