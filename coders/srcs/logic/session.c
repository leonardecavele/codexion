/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:43 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/20 03:02:01 by ldecavel         ###   ########.fr       */
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
#include "threads.h"
#include "debug.h"
#include "mutex.h"

static void	wait_session(size_t n_threads, t_session *session)
{
	size_t	i;

	i = -1;
	while (++i < n_threads)
		pthread_join(session->objects.coders[i].thread, NULL);
	pthread_join(session->monitor, NULL);
	pthread_mutex_destroy(&session->print_mutex);
	pthread_mutex_destroy(&session->queue_mutex);
	pthread_mutex_destroy(&session->dongles_mutex);
	pthread_mutex_destroy(&session->over_mutex);
	pthread_mutex_destroy(&session->ready_mutex);
	pthread_cond_destroy(&session->dongles_cond);
	debug_print(*session->args, "session ended");
}

static void	start_session(t_args *args, t_session *session)
{
	size_t	i;

	session->start_ms = current_time_ms();
	i = -1;
	while (++i < args->noc)
		session->objects.coders[i].last_compile = session->start_ms;
	debug_print(*args, "starting session");
	bool_thread_set(&session->ready_mutex, &session->ready, true);
}

static t_errcode	set_up_session(
	t_args *args, t_session *session, t_objects *objects
)
{
	size_t		i;

	if (pthread_create(&session->monitor, NULL, handle_monitor, session) != 0)
		return (THREAD_CREATE_ERROR);
	debug_print(*args, "monitor thread successfully created");
	i = -1;
	while (++i < args->noc)
	{
		if (pthread_create(
				&objects->coders[i].thread, NULL,
				handle_coder, &objects->coders[i]
			) != 0)
		{
			bool_thread_set(&session->over_mutex, &session->over, true);
			wait_session(i, session);
			return (THREAD_CREATE_ERROR);
		}
		debug_print(*args, "coder thread successfully created");
	}
	return (NO_ERROR);
}

extern t_errcode	handle_session(t_args *args, t_session *session)
{
	t_errcode		errcode;
	pthread_mutex_t	*mutexes[5];

	debug_print(*args, "initiating mutexes");
	mutexes[0] = &session->print_mutex;
	mutexes[1] = &session->dongles_mutex;
	mutexes[2] = &session->over_mutex;
	mutexes[3] = &session->ready_mutex;
	mutexes[4] = &session->queue_mutex;
	if (init_mutexes(mutexes, 5) != NO_ERROR)
		return (MUTEX_INIT_ERROR);
	if (pthread_cond_init(&session->dongles_cond, NULL) != 0)
	{
		destroy_mutexes(mutexes, 5);
		return (COND_INIT_ERROR);
	}
	debug_print(*args, "setting up session");
	errcode = set_up_session(args, session, &session->objects);
	if (errcode == NO_ERROR)
	{
		start_session(args, session);
		wait_session(args->noc, session);
	}
	destroy_coder_mutexes(&session->objects, args->noc);
	return (errcode);
}
