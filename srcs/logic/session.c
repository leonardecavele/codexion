/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:43 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 19:06:30 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>

#include "objects.h"
#include "session.h"
#include "helpers.h"
#include "args.h"
#include "error.h"
#include "coder.h"
#include "monitor.h"

extern t_status	wait_start_session(t_session *session)
{
	while (1)
	{
		if (integer_thread_cmp(&session->ready_mutex, session->ready, true))
			return (WORKING);
		if (integer_thread_cmp(&session->over_mutex, session->over, true))
			return (OVER);
		usleep(100);
	}
}

static void	wait_session(size_t n_threads, t_session *session)
{
	size_t	i;

	i = -1;
	while (++i < n_threads)
		pthread_join(session->objects.coders[i].thread, NULL);
	pthread_join(session->monitor, NULL);
	pthread_mutex_destroy(&session->print_mutex);
	pthread_mutex_destroy(&session->dongles_mutex);
	pthread_mutex_destroy(&session->over_mutex);
	pthread_mutex_destroy(&session->ready_mutex);
	pthread_cond_destroy(&session->dongles_cond);
}

static void	start_session(t_args *args, t_session *session)
{
	size_t	i;

	session->start_ms = current_time_ms();
	i = -1;
	while (++i < args->noc)
		session->objects.coders[i].last_compile = session->start_ms;
	pthread_mutex_lock(&session->ready_mutex);
	session->ready = true;
	pthread_mutex_unlock(&session->ready_mutex);
}

static t_errcode	set_up_session(
	t_args *args, t_session *session, t_objects *objects
)
{
	size_t		i;

	if (pthread_create(&session->monitor, NULL, handle_monitor, objects) != 0)
		return (THREAD_CREATE_ERROR);
	i = -1;
	while (++i < args->noc)
		if (pthread_create(
				&objects->coders[i].thread, NULL,
				handle_coder, &objects->coders[i]
			) != 0)
		{
			pthread_mutex_lock(&session->over_mutex);
			session->over = true;
			pthread_mutex_unlock(&session->over_mutex);
			wait_session(i, session);
			return (THREAD_CREATE_ERROR);
		}
	return (NO_ERROR);
}

extern t_errcode	handle_session(t_args *args, t_session *session)
{
	t_errcode errcode;

	pthread_mutex_init(&session->print_mutex, NULL);
	pthread_mutex_init(&session->dongles_mutex, NULL);
	pthread_mutex_init(&session->over_mutex, NULL);
	pthread_mutex_init(&session->ready_mutex, NULL);
	pthread_cond_init(&session->dongles_cond, NULL);
	errcode = set_up_session(args, session, &session->objects);
	if (errcode == NO_ERROR)
	{
		start_session(args, session);
		wait_session(args->noc, session);
	}
	return (errcode);
}
