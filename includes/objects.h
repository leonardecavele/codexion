/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:04:22 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 16:00:12 by ldecavel         ###   ########.fr       */
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
	bool			waiting;
	size_t			request_seq;
	t_session		*session;
	t_args			*args;
}	t_coder;

typedef struct s_objects
{
	t_coder		*coders;
	t_dongle	*dongles;
}	t_objects;

void		set_up_objects(t_args *args, t_objects *objs, t_session *session);
t_errcode	allocate_objects(t_args args, t_objects *objs);
void		destroy_objects(t_args args, t_objects *objs);

#endif
