/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda- <nucieda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:39 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/04 23:49:25 by nucieda-         ###   ########.fr       */
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
		philo[i].id = i + 1;
		philo[i].last_eat = 0;
		philo[i].last_sleep = 0;
		philo[i].left = get_fork(table, philo->id, 'L');
		philo[i].right = get_fork(table, philo->id, 'R');
		philo[i].meals = table->meals;
		philo[i].dead = 0;
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
		if(pthread_mutex_init(&forks[i], NULL) != 0)
			return (NULL);
		i++;
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
	table->meals = -1;
	if (argv[5])
		table->meals = atoi(argv[5]);
	table->forks = forks_init(table);
	table->philos = philo_init(table);
	return (table);
}

long int	timeval_to_ms(struct timeval time)
{
	long int	ms;

	ms = 0;

	ms += time.tv_sec * 1000;
	ms += time.tv_usec / 1000;
	return (ms);
}

int	check_delay(struct timeval timer)
{
	struct timeval	new;
	long int		start;
	long int		now;

	gettimeofday(&new, NULL);
	start = timeval_to_ms(timer);
	now = timeval_to_ms(new);
	return (now - start);
}

void	p_print(t_philo philo, char *s, int	ms)
{
	printf("%d\t%d %s", ms, philo.id, s);
}

int		check_dead(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->count)
	{
		if (table->philos[i].dead)
			return(1);
		i++;
	}
	return (0);
}

int	check_death(t_philo *philo, int time_to_die, int time)
{
	if (philo->last_eat > time_to_die)
	{
		p_print(*philo, "died\n", time);
		philo->dead = 1;
		return (1);
	}
	return (0);
}

int	p_eat(t_philo *philo, int time_to_eat, int	time_to_die, int time)
{
	struct timeval	timer;
	int				clock;
	
	gettimeofday(&timer, NULL);
	pthread_mutex_lock(philo->left);
	clock = check_delay(timer);
	philo->last_eat += clock;
	if (check_death(philo, time_to_die, clock + time))
		return (clock - time);
	p_print(*philo, "has taken a fork\n", clock + time);
	pthread_mutex_lock(philo->right);
	philo->last_eat -= clock;
	clock = check_delay(timer);
	philo->last_eat += clock;
	if (check_death(philo, time_to_die, clock + time))
		return (clock - time);
	p_print(*philo, "has taken a fork\n", clock + time);
	philo->last_eat -= clock;
	clock = check_delay(timer);
	philo->last_eat += clock;
	if (check_death(philo, time_to_die, clock + time))
		return (clock - time);
	p_print(*philo, "is eating\n", clock + time);
	usleep(time_to_eat * 1000);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	philo->last_eat = 0;
	clock = check_delay(timer);
	return (clock - time);
}

int	p_sleep(t_philo *philo, int	time_to_sleep, int time)
{
	struct timeval	timer;
	int				clock;

	gettimeofday(&timer, NULL);
	clock = check_delay(timer);
	p_print(*philo, "is sleeping\n", clock + time);
	usleep(time_to_sleep * 1000);
	philo->last_eat += time_to_sleep;
	p_print(*philo, "is thinking\n", clock + time + time_to_sleep);
	clock = check_delay(timer);
	return (clock - time);	
}

void	*exist(void	*arg)
{
	t_table	*table;
	struct	timeval timer;
	int		id;
	int		clock;
	int		offset;

	id = **(int **)arg;
	table = ((t_table **)arg)[1];
	gettimeofday(&timer, NULL);
	offset = -1;
	if (table->philos[id].meals == -1)
		offset = 0;
	while(table->philos[id].meals != 0)
	{
		clock = check_delay(timer);
		if (check_dead(table))
			break;
		p_eat(&table->philos[id], table->eat, table->die, clock);
		clock = check_delay(timer);
		if (check_dead(table))
			break;
		p_sleep(&table->philos[id], table->sleep, clock);
		table->philos[id].meals += offset;
	}
	free(*(void **)arg);
	free(arg);
	return (NULL);
}

void	start_sim(t_table *table)
{
	void	**id;
	int	i;

	i = 0;
	while (i < table->count)
	{
		id = malloc(2 * sizeof(void *));
		id[0] = malloc(sizeof(int));
		*(int *)id[0] = i;
		id[1] = (void *)table;
		if (pthread_create(&table->philos[i].th, NULL, &exist, id) != 0)
			exit(5);
		i++;
	}
	while (--i >= 0)
	{
		if (pthread_join(table->philos[i].th, NULL) != 0)
			exit(6);
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