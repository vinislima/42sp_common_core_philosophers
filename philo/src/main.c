/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:12:40 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/18 23:31:23 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Clean up all allocated resources and destroy mutexes
 * @param sim Pointer to simulation structure to cleanup
 */
void	cleanup_simulation(t_simulation *sim)
{
	int	i;
	
	if (sim->philosophers)
	{
		i = 0;
		while (i < sim->num_philosophers)
		{
			pthread_mutex_destroy(&sim->philosophers[i].meal_lock);
			i++;
		}
		free(sim->philosophers);
	}
	if (sim->forks)
	{
		i = 0;
		while (i < sim->num_philosophers)
		{
			pthread_mutex_destroy(&sim->forks[i]);
			i++;
		}
		free(sim->forks);
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->sim_lock);
}

/**
 * @brief Print usage instructions for the program
 */
static void	print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers time_do_die ");
	printf("time_to_eat time_to_sleep ");
	printf("[number_of_time_each_philosopher_must_eat]\n");
	printf("Arguments must be positive integers\n");
}

/**
 * @brief Main function: entry point of philosophers program
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Returns 0 on success, 1 on failure
 */
int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (!init_simulation(&sim, argc, argv))
	{
		print_usage();
		return (1);
	}
	if (!start_simulation(&sim))
	{
		cleanup_simulation(&sim);
		printf("Error: Failed to start simulation\n");
		return (1);
	}
	cleanup_simulation(&sim);
	return (0);
}
