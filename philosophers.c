/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda <nucieda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:39 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/25 18:53:51 by nucieda          ###   ########.fr       */
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
		philo[i].left = get_fork(table, philo[i].id, 'L');
		philo[i].right = get_fork(table, philo[i].id, 'R');
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
	table->dead = 0;
	table->forks = forks_init(table);
	pthread_mutex_init(&table->death, NULL);
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

	start = timeval_to_ms(timer);
	gettimeofday(&new, NULL);
	now = timeval_to_ms(new);
	return (now - start);
}

void	p_print(t_philo philo, char *s, t_table *table)
{
	int	time;

	time = check_delay(table->timer);
	if (s[0] == 'd')
		usleep(100);
	if (!check_dead(table))
		printf("%d\t%d %s", time, philo.id, s);
}

int		check_dead(t_table *table)
{
	return (table->dead);
}

int	check_death(t_philo *philo, t_table *table)
{
	int	time_since_start;

	time_since_start = check_delay(table->timer);
	if (philo->last_eat < (time_since_start - table->die))
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
	struct timeval	timer;
	
	gettimeofday(&timer, NULL);
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
		return (0);
	p_print(*philo, "is eating\n", table);
	philo->last_eat = check_delay(table->timer);
	usleep(table->eat * 1000);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
	return (0);
}

int	p_sleep(t_philo *philo, t_table *table)
{

	if (check_death(philo, table))
		return (0);
	p_print(*philo, "is sleeping\n", table);
	usleep(table->sleep * 1000);
	if (check_death(philo, table))
		return (0);
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
		usleep(table->eat * 1000);
	while(table->philos[id].meals != 0)
	{
		if (check_dead(table))
			break;
		p_eat(&table->philos[id], table);
		if (check_dead(table))
			break;
		p_sleep(&table->philos[id], table);
		table->philos[id].meals += offset;
	}
	free(*(void **)arg);
	free(arg);
	return (NULL);
}

void	one_philo(t_table *table)
{
	printf("0\t1 has taken a fork\n");
	usleep(table->die * 1000);
	printf("%d\t1 died\n", table->die);
}

void	start_sim(t_table *table)
{
	void	**id;
	int	i;

	i = 0;
	gettimeofday(&table->timer, NULL);
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
	while (i < table->count)
	{
		if (pthread_mutex_destroy(&table->forks[i]) != 0)
			exit(7);
	}
}

int main(int argc, char *argv[])
{
	t_table	*table;

	if (argc < 5 || argc > 6)
		return (1);
	table = table_init(argv);
	if (table->count == 1)
		one_philo(table);
	else
		start_sim(table);
}