/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:04:22 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/17 18:36:34 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stddef.h>
# include <pthread.h>

# include "args.h"
# include "error.h"

typedef struct s_session	t_session;

typedef struct s_dongle
{
	size_t			id;
	size_t			last_use;
	bool			available;
	// 2 coders table priority queue
}	t_dongle;

typedef struct s_coder
{
	size_t		id;
	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	t_session	*session;
	t_args		*args;
}	t_coder;

typedef struct s_objects
{
	t_coder		*coders;
	t_dongle	*dongles;
}	t_objects;

void		set_up_objects(t_args *args, t_objects *objs, t_session *session);
t_errcode	allocate_objects(t_args args, t_objects *objs);
void		destroy_objects(t_objects *objs);

#endif
