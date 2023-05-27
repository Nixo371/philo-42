/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda- <nucieda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 20:22:08 by nucieda-          #+#    #+#             */
/*   Updated: 2023/05/27 20:31:40 by nucieda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo, t_table *table)
{
	if (philo->last_eat < (check_delay(table->timer) - table->die))
	{
		pthread_mutex_lock(&table->death);
		p_print(*philo, "died\n", table);
		table->dead = 1;
		pthread_mutex_unlock(&table->death);
		return (1);
	}
	return (0);
}

int	grab_forks(t_philo *philo, t_table *table)
{
	pthread_mutex_lock(philo->left);
	if (check_death(philo, table))
	{
		pthread_mutex_unlock(philo->left);
		return (0);
	}
	p_print(*philo, "has taken a fork\n", table);
	pthread_mutex_lock(philo->right);
	if (check_death(philo, table))
	{
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		return (0);
	}
	p_print(*philo, "has taken a fork\n", table);
	return (1);
}

int	p_eat(t_philo *philo, t_table *table)
{
	if (check_death(philo, table) || !grab_forks(philo, table))
		return (1);
	p_print(*philo, "is eating\n", table);
	philo->last_eat = check_delay(table->timer);
	ft_usleep(table->eat * 1000, philo, table);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	return (0);
}

int	p_sleep(t_philo *philo, t_table *table)
{
	if (check_death(philo, table))
		return (1);
	p_print(*philo, "is sleeping\n", table);
	ft_usleep(table->sleep * 1000, philo, table);
	if (check_death(philo, table))
		return (1);
	p_print(*philo, "is thinking\n", table);
	return (0);
}

void	*exist(void	*arg)
{
	t_table	*table;
	int		id;
	int		offset;

	id = **(int **)arg;
	table = ((t_table **)arg)[1];
	offset = -1;
	if (table->philos[id].meals == -1)
		offset = 0;
	if (id % 2 == 0)
		ft_usleep((table->eat - 10) * 1000, &table->philos[id], table);
	while (table->philos[id].meals != 0)
	{
		if (table->dead)
			break ;
		if (p_eat(&table->philos[id], table))
			break ;
		if (p_sleep(&table->philos[id], table))
			break ;
		table->philos[id].meals += offset;
	}
	free(*(void **)arg);
	free(arg);
	return (NULL);
}
