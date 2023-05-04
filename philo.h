/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nucieda <nucieda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 20:13:53 by nucieda           #+#    #+#             */
/*   Updated: 2023/05/04 11:34:39 by nucieda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>

typedef struct s_philosophers {
	pthread_t	*th;
	int			*time_since_eat;
	int			*time_since_sleep;
	int			num;
}	t_philosophers;

typedef struct s_forks
{
	pthread_mutex_t *forks;
	int				num;
}	t_forks;


#endif