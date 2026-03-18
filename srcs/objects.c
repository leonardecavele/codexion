/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:09:00 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 18:08:13 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>

#include "args.h"
#include "error.h"
#include "objects.h"
#include "session.h"


extern void	set_up_objects(t_args *args, t_objects *objs, t_session *session)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
	{
		objs->coders[i].left = &objs->dongles[
			(i + args->noc - 1) % args->noc
		];
		objs->dongles[i].available = true;
		objs->dongles[i].id = i + 1;
		objs->dongles[i].last_use = 0;
		objs->coders[i].right = &objs->dongles[i % args->noc];
		objs->coders[i].id = i + 1;
		objs->coders[i].args = args;
		objs->coders[i].session = session;
		pthread_mutex_init(&objs->coders[i].over_mutex, NULL);
		pthread_mutex_init(&objs->coders[i].last_compile_mutex, NULL);
	}
	return ;
}

extern t_errcode	allocate_objects(t_args args, t_objects *objs)
{
	objs->coders = malloc(sizeof(t_coder) * args.noc);
	if (!objs->coders)
		return (MALLOC_ERROR);
	objs->dongles = malloc(sizeof(t_dongle) * args.noc);
	if (!objs->dongles)
		return (MALLOC_ERROR);
	return (NO_ERROR);
}

extern void	destroy_objects(t_objects *objs)
{
	free(objs->coders);
	free(objs->dongles);
	return ;
}
