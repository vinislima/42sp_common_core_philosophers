/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 22:43:13 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/18 22:00:58 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Validate command line arguments format and count
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Returns 1 if arguments are valid, 0 otherwise
 */
static int	validate_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		return (0);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		if (j == 0)
			return (0);
		i++;
	}
	return (1); 
}

/**
 * @brief Parse and store command line arguments into simulation structure
 * @param sim Pointer to simulation structure to store parsed values
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments strings
 * @return Returns 1 if parsing successful, 0 if invalid values
 */
static int	parse_arguments(t_simulation *sim, int argc, char **argv)
{
	sim->num_philosophers = atoi(argv[1]);
	sim->time_to_die = atoi(argv[2]);
	sim->time_to_eat = atoi(argv[3]);
	sim->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		sim->num_meals_required = atoi(argv[5]);
	else
		sim->num_meals_required = -1;
	if (sim->num_philosophers <= 0 || sim->time_to_die <= 0
		|| sim->time_to_eat <= 0 || sim->time_to_sleep <= 0)
		return (0);
	if (argc == 6 && sim->num_meals_required <= 0)
		return (0);
	return (1);
}

/**
 * @brief Initialize all mutexes for the simulation
 * @param sim Pointer to simulation structure containing mutex arrays
 * @return Returns 1 if all mutexes initialized sucessfully, 0 on failure
 */
static int	init_mutexes(t_simulation *sim)
{
	int	i;

	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->sim_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_lock);
		return (0);
	}
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&sim->forks[i]);
			pthread_mutex_destroy(&sim->print_lock);
			pthread_mutex_destroy(&sim->sim_lock);
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Initialize all philosopher structures with default values and fork assignments
 * @param sim Pointer to simulation structure containing philosopher array
 */
static void	init_philoshophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].meals_eaten = 0;
		sim->philosophers[i].last_meal_time = 0;
		sim->philosophers[i].left_fork = &sim->forks[i];
		sim->philosophers[i].right_fork = &sim->forks[(i + 1)
			% sim->num_philosophers];
		pthread_mutex_init(&sim->philosophers[i].meal_lock, NULL);
		sim->philosophers[i].simulation = sim;
		i++;
	}
}

/**
 * @brief Initialize complete simulation structure with all components
 * @param sim Pointer to simulation structure to initialize
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments strings
 * @return Returns 1 if initialization successful, 0 on failure
 */
int	init_simulation(t_simulation *sim, int argc, char **argv)
{
	if (!validate_args(argc, argv))
		return (0);
	if (!parse_arguments(sim, argc, argv))
		return (0);
	sim->philosophers = malloc(sizeof(t_philosopher)
			* sim->num_philosophers);
	if (!sim->philosophers)
		return (0);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->forks)
	{
		free(sim->philosophers);
		return (0);
	}
	if (!init_mutexes(sim))
	{
		free(sim->philosophers);
		free(sim->forks);
		return (0);
	}
	sim->simulation_end = 0;
	sim->start_time = get_time();
	init_philoshophers(sim);
	return (1);
}
