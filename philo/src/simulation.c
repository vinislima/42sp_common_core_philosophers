/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:44:42 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/21 20:03:05 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Philosopher thinking routine
 * @param philo Pointer to philosopher
 */
static void	think(t_philosopher *philo)
{
	safe_print(philo->simulation, philo->id, "is thinking");
}

/**
 * @brief Philosopher eating routine with meal tracking
 * @param philo Pointer to philosopher structure
 */
static void	eat(t_philosopher *philo)
{
	safe_print(philo->simulation, philo->id, "is eating");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	smart_sleep(philo->simulation->time_to_eat);
}

/**
 * @brief Philosopher sleeping routine
 * @param philo Pointer to philosopher structure
 */
static void	sleep_philo(t_philosopher *philo)
{
	safe_print(philo->simulation, philo->id, "is sleeping");
	smart_sleep(philo->simulation->time_to_sleep);
}

/**
 * @brief Main philosopher routine executed by each thread
 * @param arg Pointer to philosopher structure (void* for pthread compatibility)
 * @return Returns NULL when routine completes
 */
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal_time = philo->simulation->start_time;
	pthread_mutex_unlock(&philo->meal_lock);
	if (philo->simulation->num_philosophers == 1)
	{
		safe_print(philo->simulation, philo->id, "has taken a fork");
		smart_sleep(philo->simulation->time_to_die);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		smart_sleep(philo->simulation->time_to_eat / 2);
	while (!check_simulation_end(philo->simulation))
	{
		think(philo);
		acquire_forks(philo);
		if (!check_simulation_end(philo->simulation))
		{
			eat(philo);
			release_forks(philo);
			sleep_philo(philo);
		}
	}
	return (NULL);
}

/**
 * @brief Start simulation by creating and managing philosopher threads
 * @param sim Pointer to simulation structure
 * @return Returns 1 on success, 0 on failure
 */
int	start_simulation(t_simulation *sim)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim) != 0)
		return (0);
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_create(&sim->philosophers[i].thread, NULL,
				philosopher_routine, &sim->philosophers[i]) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_join(sim->philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
	return (1);
}
