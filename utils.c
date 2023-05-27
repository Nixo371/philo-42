/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda- <nucieda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 20:30:13 by nucieda-          #+#    #+#             */
/*   Updated: 2023/05/27 20:58:50 by nucieda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	timeval_to_ms(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
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

void	ft_usleep(long int us, t_philo *philo, t_table *table)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	while (check_delay(time) < us / 1000)
	{
		if (check_death(philo, table))
			return ;
		usleep(us / 1000);
	}
}

void	p_print(t_philo philo, char *s, t_table *table)
{
	int	time;

	time = check_delay(table->timer);
	if (s[0] != 'd')
		usleep(50);
	else
		time = philo.last_eat + table->die;
	if (!table->dead)
		printf("%d %d %s", time, philo.id, s);
}

int	ft_atoi(const char *ptr)
{
	int	i;
	int	num;
	int	neg;

	i = 0;
	num = 0;
	neg = 1;
	while (ptr[i] == ' ' || (ptr[i] >= 9 && ptr[i] <= 13))
		i++;
	if (ptr[i] == '+')
		i++;
	else if (ptr[i] == '-')
	{
		neg *= -1;
		i++;
	}
	while (ptr[i] != '\0')
	{
		if (ptr[i] < '0' || ptr[i] > '9')
			return (num * neg);
		num = (num * 10) + ptr[i] - 48;
		i++;
	}
	return (num * neg);
}
