/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:25:05 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/22 22:33:23 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Philosopher thinking routine
 * @param philo Pointer to philosopher
 */
void	think(t_philosopher *philo)
{
	safe_print(philo->simulation, philo->id, "is thinking");
}

/**
 * @brief Philosopher eating routine with meal tracking
 * @param philo Pointer to philosopher structure
 */
void	eat(t_philosopher *philo)
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
void	sleep_philo(t_philosopher *philo)
{
	safe_print(philo->simulation, philo->id, "is sleeping");
	smart_sleep(philo->simulation->time_to_sleep);
}