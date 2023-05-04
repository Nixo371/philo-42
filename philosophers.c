/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda <nucieda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:39 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/04 11:37:17 by nucieda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*eat()
{
	
}

int main(int argc, char *argv[])
{
	t_philosophers	p;
	t_forks			f;

	int	die;
	int	eat;
	int	sleep;
	int	eats;
	int	i;

	if (argc < 5 || argc > 6)
		return (0);
	p.num = atoi(argv[1]);
	die = atoi(argv[2]);
	eat = atoi(argv[3]);
	sleep = atoi(argv[4]);
	if (argc == 6)
		eats = atoi(argv[5]);
	printf("Number of philosophers: %d\n", p.num);
	printf("Time to die (in ms): %d\n", die);
	printf("Time to eat (in ms): %d\n", eat);
	printf("Time to sleep (in ms): %d\n", sleep);
	if (argc == 6)
		printf("Number of times needed to eat: %d\n", eats);
	i = 0;
	p.th = malloc(p.num * sizeof(pthread_t));
	f.forks = malloc(f.num * sizeof(pthread_mutex_t));
	while (i < p.num)
	{
		if (pthread_create(p.th[i], NULL, &eat, NULL));
			return (1);
		if (pthread_mutex_init(&f.forks[i], NULL));
			return (2);
		i++;
	}
	while (--i >= 0)
	{
		if (pthread_join(p.th[i], NULL))
			return (3);
		if (pthread_mutex_destroy(&f.forks[i]));
	}
}