/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:04:22 by ldecavel          #+#    #+#             */
/*   Updated: 2026/04/04 11:05:31 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include <pthread.h>
# include <stdbool.h>

# include "error.h"

typedef struct s_session	t_session;
typedef struct s_args		t_args;
typedef struct s_coder		t_coder;

typedef struct s_dongle
{
	size_t			id;
	size_t			last_use;
	bool			available;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_coder			*heap[2];
}	t_dongle;

typedef struct s_coder
{
	size_t			id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	pthread_mutex_t	last_compile_mutex;
	size_t			last_compile;
	pthread_mutex_t	over_mutex;
	bool			over;
	pthread_mutex_t waiting_mutex;
	bool			waiting;
	pthread_mutex_t ticket_mutex;
	size_t			ticket;
	t_session		*session;
	t_args			*args;
}	t_coder;

typedef struct s_objects
{
	t_coder		*coders;
	t_dongle	*dongles;
}	t_objects;

t_errcode	set_up_objects(t_args *args, t_objects *objs, t_session *session);
t_errcode	allocate_objects(t_args args, t_objects *objs);
void		destroy_objects(t_args args, t_objects *objs);

#endif
