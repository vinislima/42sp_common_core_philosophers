/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:18:09 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/21 22:59:57 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Take left fork with proper deadlock avoindance
 * @param philo Pointer to philosopher structure
 */
static void	take_left_fork(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_print(philo->simulation, philo->id, "has taken a fork");
}

/**
 * @brief Take right fork with proper deadlock avoidance
 * @param philo Pointer to philosopher structure
 */
static void	take_right_fork(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_fork);
	safe_print(philo->simulation, philo->id, "has taken a fork");
}

/**
 * @brief Handle first fork acquisition and check simulation
 * @param philo Pointer to philosopher structure
 * @param first_fork Pointer to the first fork mutex
 * @return 1 if should continue, 0 if should stop
 */
static int	take_first_fork(t_philosopher *philo, pthread_mutex_t *first_fork)
{
	if (first_fork == philo->left_fork)
		take_left_fork(philo);
	else
		take_right_fork(philo);
	if (check_simulation_end(philo->simulation))
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	return (1);
}

/**
 * @brief Acquire both forks using hierarchy to prevent deadlock
 * @param philo Pointer to philosopher structure
 */
void	acquire_forks(t_philosopher *philo)
{
	int				left_index;
	int				right_index;
	pthread_mutex_t	*first_fork;

	if (check_simulation_end(philo->simulation))
		return ;
	left_index = philo->id - 1;
	right_index = philo->id % philo->simulation->num_philosophers;
	if (left_index < right_index)
		first_fork = philo->left_fork;
	else
		first_fork = philo->right_fork;
	if (!take_first_fork(philo, first_fork))
		return ;
	if (first_fork == philo->left_fork)
		take_right_fork(philo);
	else
		take_left_fork(philo);
}

/**
 * @brief Release both forks in reverse order
 * @param philo Pointer to philosopher structure
 */
void	release_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
