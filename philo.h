/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda <nucieda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:53 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/04 21:24:40 by nucieda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>

typedef struct s_philo
{
	int				id;
	int				last_eat;
	int				last_sleep;
	int				meals;
	pthread_t		th;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
}	t_philo;

typedef struct s_table
{
	t_philo 		*philos;
	pthread_mutex_t	*forks;
	int				count;
	int				die;
	int				eat;
	int				sleep;
	int				meals;
} t_table;



#endif