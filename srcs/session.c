/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:43 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 17:38:45 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>

#include "objects.h"
#include "session.h"
#include "helpers.h"
#include "args.h"
#include "error.h"
#include "coder.h"
#include "monitor.h"

static void	enable_session(t_args *args, t_session *session)
{
	size_t	i;

	session->start_ms = current_time_ms();
	i = -1;
	while (++i < args->noc)
		session->objects.coders[i].last_compile = session->start_ms;
	session->ready = true;
}

extern t_errcode	start_session(t_args *args, t_session *session)
{
	size_t		i;
	t_objects	*objects;

	objects = &session->objects;
	pthread_mutex_init(&session->print_mutex, NULL);
	pthread_mutex_init(&session->dongles_mutex, NULL);
	pthread_mutex_init(&session->killed_mutex, NULL);
	pthread_cond_init(&session->dongles_cond, NULL);
	if (pthread_create(&session->monitor, NULL, handle_monitor, objects) != 0)
		return (THREAD_CREATE_ERROR);
	i = -1;
	while (++i < args->noc)
	{
		if (pthread_create(
				&objects->coders[i].thread, NULL,
				handle_coder, &objects->coders[i]
			) != 0)
		{
			pthread_mutex_lock(&session->killed_mutex);
			session->killed = true;
			pthread_mutex_unlock(&session->killed_mutex);
			wait_session(i, session);
			return (THREAD_CREATE_ERROR);
		}
	}
	enable_session(args, session);
	return (NO_ERROR);
}

extern void	wait_session(size_t n_threads, t_session *session)
{
	size_t	i;

	i = -1;
	while (++i < n_threads)
		pthread_join(session->objects.coders[i].thread, NULL);
	pthread_join(session->monitor, NULL);
	pthread_mutex_destroy(&session->print_mutex);
	pthread_mutex_destroy(&session->dongles_mutex);
}
