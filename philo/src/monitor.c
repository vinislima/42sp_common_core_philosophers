/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:32:50 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/19 00:01:36 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Check if a philosopher has died from starvation
 * @param sim Pointer to simulation structure
 * @param i Index of philosopher to check
 * @return Returns 1 if philosopher died, 0 otherwise
 */
static int	check_death(t_simulation *sim, int i)
{
	long	current_time;
	long	time_since_meal;

	current_time = get_time();
	pthread_mutex_lock(&sim->philosophers[i].meal_lock);
	time_since_meal = current_time - sim->philosophers[i].last_meal_time;
	pthread_mutex_unlock(&sim->philosophers[i].meal_lock);
	if (time_since_meal > sim->time_to_die)
	{
		pthread_mutex_lock(&sim->print_lock);
		printf("%ld %d died\n", current_time - sim->start_time,
			sim->philosophers[i].id);
		pthread_mutex_unlock(&sim->print_lock);
		return (1);
	}
	return (0);
}

/**
 * @brief Check if all philosophers have eaten required meals
 * @param sim Pointer to simulation structure
 * @return Returns 1 if all have enough, 0 otherwise
 */
static int	check_meals_complete(t_simulation *sim)
{
	int	i;
	int	complete_philosophers;

	if (sim->num_meals_required == -1)
		return (0);
	complete_philosophers = 0;
	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_mutex_lock(&sim->philosophers[i].meal_lock);
		if (sim->philosophers[i].meals_eaten >= sim->num_meals_required)
			complete_philosophers++;
		pthread_mutex_unlock(&sim->philosophers[i].meal_lock);
		i++;
	}
	return (complete_philosophers == sim->num_philosophers);
}

/**
 * @brief Check all philosophers for death
 * @param sim Pointer to simulation structure
 * @return Returns 1 if any philosopher died, 0 otherwise
 */
static int	check_all_deaths(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		if (check_death(sim, i))
		{
			pthread_mutex_lock(&sim->sim_lock);
			sim->simulation_end = 1;
			pthread_mutex_unlock(&sim->sim_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Monitor routine to check for death and meal completation
 * @param arg Pointer to simulation structure (void* for pthread compatibility)
 * @return Returns NULL when monitoring completes
 */
void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (!check_simulation_end(sim))
	{
		if (check_all_deaths(sim))
			return (NULL);
		if (check_meals_complete(sim))
		{
			pthread_mutex_lock(&sim->sim_lock);
			sim->simulation_end = 1;
			pthread_mutex_unlock(&sim->sim_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}