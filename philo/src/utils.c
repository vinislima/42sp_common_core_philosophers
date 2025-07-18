/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:20:47 by vinda-si          #+#    #+#             */
/*   Updated: 2025/07/17 23:29:04 by vinda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long duration)
{
	long	start_time;
	long	current_time;

	start_time = get_time();
	current_time = start_time;
	while ((current_time - start_time) < duration)
	{
		usleep(100);
		current_time = get_time();
	}
}

void	safe_print(t_simulation *sim, int id, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&sim->print_lock);
	if (!check_simulation_end(sim))
	{
		timestamp = get_time() - sim->start_time;
		printf("%ld %d %s\n", timestamp, id, message);
	}
	pthread_mutex_unlock(&sim->print_lock);
}

int	check_simulation_end(t_simulation *sim)
{
	int	end;

	pthread_mutex_lock(&sim->sim_lock);
	end = sim->simulation_end;
	pthread_mutex_unlock(&sim->sim_lock);
	return (end);
}