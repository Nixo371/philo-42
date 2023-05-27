/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda- <nucieda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 20:28:25 by nucieda-          #+#    #+#             */
/*   Updated: 2023/05/27 20:57:31 by nucieda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*get_fork(t_table *table, int id, char sel)
{
	if (sel == 'L')
		return (&table->forks[id - 1]);
	else
	{
		if (id == table->count)
			return (&table->forks[0]);
		else
			return (&table->forks[id]);
	}
}

t_philo	*philo_init(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = malloc(table->count * sizeof(t_philo));
	i = 0;
	while (i < table->count)
	{
		philo[i].id = i + 1;
		philo[i].last_eat = 0;
		if (philo[i].id % 2)
		{
			philo[i].left = get_fork(table, philo[i].id, 'L');
			philo[i].right = get_fork(table, philo[i].id, 'R');
		}
		else
		{
			philo[i].left = get_fork(table, philo[i].id, 'R');
			philo[i].right = get_fork(table, philo[i].id, 'L');
		}
		philo[i].meals = table->meals;
		i++;
	}
	return (philo);
}

pthread_mutex_t	*forks_init(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(table->count * sizeof(pthread_mutex_t));
	i = 0;
	while (i < table->count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (NULL);
		i++;
	}
	return (forks);
}

t_table	*table_init(char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	table->count = ft_atoi(argv[1]);
	table->die = ft_atoi(argv[2]);
	table->eat = ft_atoi(argv[3]);
	table->sleep = ft_atoi(argv[4]);
	table->meals = -1;
	if (argv[5])
		table->meals = ft_atoi(argv[5]);
	table->dead = 0;
	table->forks = forks_init(table);
	pthread_mutex_init(&table->death, NULL);
	table->philos = philo_init(table);
	if (table->count < 0 || table->die < 0 || table->eat < 0)
		return (NULL);
	if (table->sleep < 0 || (argv[5] && table->meals < 0))
		return (NULL);
	return (table);
}
