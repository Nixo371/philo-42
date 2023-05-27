/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda- <nucieda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:53 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/27 20:59:48 by nucieda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				last_eat;
	int				meals;
	pthread_t		th;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
}	t_philo;

typedef struct s_table
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death;
	struct timeval	timer;
	int				dead;
	int				count;
	int				die;
	int				eat;
	int				sleep;
	int				meals;
}	t_table;

pthread_mutex_t	*get_fork(t_table *table, int id, char sel);
t_philo			*philo_init(t_table *table);
pthread_mutex_t	*forks_init(t_table *table);
t_table			*table_init(char *argv[]);
long int		timeval_to_ms(struct timeval time);
int				check_delay(struct timeval timer);
void			p_print(t_philo philo, char *s, t_table *table);
int				check_death(t_philo *philo, t_table *table);
int				grab_forks(t_philo *philo, t_table *table);
int				p_eat(t_philo *philo, t_table *table);
int				p_sleep(t_philo *philo, t_table *table);
void			*exist(void	*arg);
void			start_sim(t_table *table);
void			ft_usleep(long int us, t_philo *philo, t_table *table);
void			one_philo(t_table *table);
int				check_input(int argc, char *argv[]);
int				ft_atoi(const char *ptr);

#endif