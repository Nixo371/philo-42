/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda- <nucieda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:39 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/27 20:52:45 by nucieda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_input(int argc, char *argv[])
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void	start_sim(t_table *table)
{
	void	**id;
	int		i;

	i = -1;
	gettimeofday(&table->timer, NULL);
	while (++i < table->count)
	{
		id = malloc(2 * sizeof(void *));
		id[0] = malloc(sizeof(int));
		*(int *)id[0] = i;
		id[1] = (void *)table;
		if (pthread_create(&table->philos[i].th, NULL, &exist, id) != 0)
			exit(5);
	}
	while (--i >= 0)
	{
		if (pthread_join(table->philos[i].th, NULL) != 0)
			exit(6);
	}
	while (++i < table->count)
	{
		if (pthread_mutex_destroy(&table->forks[i]) != 0)
			exit(7);
	}
	pthread_mutex_destroy(&table->death);
}

int	main(int argc, char *argv[])
{
	t_table	*table;

	if (argc < 5 || argc > 6)
		return (1);
	if (check_input(argc, argv))
		return (0);
	table = table_init(argv);
	if (table == NULL)
		return (0);
	if (table->count == 1)
		one_philo(table);
	else
		start_sim(table);
	free(table->forks);
	free(table->philos);
	free(table);
}
