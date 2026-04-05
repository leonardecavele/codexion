/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:09:00 by ldecavel          #+#    #+#             */
/*   Updated: 2026/04/05 11:39:31 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>

#include "args.h"
#include "error.h"
#include "objects.h"
#include "session.h"
#include "debug.h"
#include "mutex.h"

static void	set_up_object(
	t_args *args, t_objects *objs, t_session *session, size_t i
)
{
	objs->coders[i].left = &objs->dongles[(i + args->noc - 1) % args->noc];
	objs->dongles[i].available = true;
	objs->dongles[i].id = i + 1;
	objs->dongles[i].last_use = 0;
	objs->dongles[i].heap[0] = &objs->coders[i];
	objs->dongles[i].heap[1] = &objs->coders[(i + 1) % args->noc];
	objs->coders[i].right = &objs->dongles[i % args->noc];
	objs->coders[i].id = i + 1;
	objs->coders[i].args = args;
	objs->coders[i].session = session;
	objs->coders[i].over = false;
	objs->coders[i].waiting = false;
	objs->coders[i].ticket = 0;
	objs->coders[i].last_compile = 0;
}

extern t_errcode	set_up_objects(
	t_args *args, t_objects *objs, t_session *session
)
{
	pthread_mutex_t	*mutexes[5];
	size_t			i;

	debug_print(*args, "setting up objects");
	i = -1;
	while (++i < args->noc)
	{
		set_up_object(args, objs, session, i);
		mutexes[0] = &objs->coders[i].over_mutex;
		mutexes[1] = &objs->coders[i].last_compile_mutex;
		mutexes[2] = &objs->coders[i].waiting_mutex;
		mutexes[3] = &objs->coders[i].ticket_mutex;
		mutexes[4] = &objs->dongles[i].mutex;
		if (pthread_cond_init(&objs->dongles[i].cond, NULL) != 0)
		{
			destroy_coder_mutexes(objs, i);
			return (COND_INIT_ERROR);
		}
		if (init_mutexes(mutexes, 5) != NO_ERROR)
		{
			destroy_coder_mutexes(objs, i);
			return (MUTEX_INIT_ERROR);
		}
	}
	return (NO_ERROR);
}

extern t_errcode	allocate_objects(t_args args, t_objects *objs)
{
	debug_print(args, "allocating objects");
	objs->coders = malloc(sizeof(t_coder) * args.noc);
	if (!objs->coders)
		return (MALLOC_ERROR);
	objs->dongles = malloc(sizeof(t_dongle) * args.noc);
	if (!objs->dongles)
		return (MALLOC_ERROR);
	debug_print(args, "objects sucessfully allocated");
	return (NO_ERROR);
}

extern void	destroy_objects(t_args args, t_objects *objs)
{
	free(objs->coders);
	free(objs->dongles);
	debug_print(args, "objects freed");
}
