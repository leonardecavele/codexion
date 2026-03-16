/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:04:22 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 11:46:53 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stddef.h>

#include "args.h"
#include "error.h"

typedef struct s_dongle
{
	size_t			id;
	pthread_mutex_t	mutex;
	bool			taken;
}	t_dongle;

typedef struct s_coder
{
	size_t		id;
	pthread_t	thread;
	t_dongle	*left;
	t_dongle	*right;
	size_t		compiled;
}	t_coder;

typedef struct s_objects
{
	t_coder		*coders;
	t_dongle	*dongles;
}	t_objects;

void		set_up_objects(t_args args, t_objects *objects);
t_errcode	allocate_objects(t_args args, t_objects *objects);
void		destroy_objects(t_objects *objects);

#endif
