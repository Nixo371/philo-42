/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda <nucieda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:39 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/04 21:44:14 by nucieda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t *get_fork(t_table *table, int id, char	sel)
{
	if (sel == 'L')
	{
		// Forks to the left of philosophers share their index
		// philo[0]'s left fork would be forks[0]
		return(&table->forks[id - 1]);
	}
	else
	{
		if (id == table->count) // Last philosopher gets forks[0]
			return(&table->forks[0]);
		else
			return(&table->forks[id]); 
	}
}

t_philo *philo_init(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = malloc(table->count * sizeof(t_philo));
	i = 0;
	while(i < table->count)
	{
		philo->id = i + 1;
		philo->last_eat = 0;
		philo->last_sleep = 0;
		philo->left = get_fork(table, philo->id, 'L');
		philo->right = get_fork(table, philo->id, 'R');
		philo->meals = 0;
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
		if(pthread_mutex_init(&forks[i], NULL) != 0)
			return (NULL);
	}
	return (forks);
}

t_table	*table_init(char *argv[])
{
	t_table *table;
	
	table = malloc(sizeof(t_table));
	table->count = atoi(argv[1]);
	table->die = atoi(argv[2]);
	table->eat = atoi(argv[3]);
	table->sleep = atoi(argv[4]);
	if (argv[5])
		table->meals = atoi(argv[5]);
	table->forks = forks_init(table);
	table->philos = philo_init(table);
	return (table);
}

void	start_sim(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->count)
	{
		
	}
}

int main(int argc, char *argv[])
{
	t_table	*table;

	if (argc < 5 || argc > 6)
		return (1);
	table = table_init(argv);
	start_sim(table);
}