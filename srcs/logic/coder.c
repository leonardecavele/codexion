/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:40:19 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 23:09:52 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "objects.h"
#include "args.h"
#include "coder.h"
#include "session.h"
#include "helpers.h"
#include "monitor.h"
#include "threads.h"
#include "queue.h"

static bool	dongles_cooled_down(t_coder *coder)
{
	if (elapsed_time_ms(coder->left->last_use) < coder->args->dc)
		return (false);
	if (elapsed_time_ms(coder->right->last_use) < coder->args->dc)
		return (false);
	return (true);
}

static t_status	take_dongles(t_coder *coder, t_session *session)
{
	while (1)
	{
		pthread_mutex_lock(&session->dongles_mutex);
		queue_enter(coder);
		if (!bool_thread_cmp(&session->over_mutex, &session->over, true))
		{
			queue_leave(coder);
			pthread_mutex_unlock(&session->dongles_mutex);
			return (OVER);
		}
		if (!coder->left->available || !coder->right->available)
		{
			pthread_cond_wait(&session->dongles_cond, &session->dongles_mutex);
			pthread_mutex_unlock(&session->dongles_mutex);
			continue ;
		}
		if (!dongles_cooled_down(coder))
		{
			pthread_mutex_unlock(&session->dongles_mutex);
			usleep(100);
			continue ;
		}
		if (queue_can_take(coder))
		{
			coder->left->available = false;
			coder->right->available = false;
			queue_leave(coder);
			pthread_mutex_unlock(&session->dongles_mutex);
			break ;
		}
		pthread_cond_wait(&session->dongles_cond, &session->dongles_mutex);
		pthread_mutex_unlock(&session->dongles_mutex);
	}
	if (log_activity(
		session->start_ms, "has taken a dongle", coder, 0) == OVER)
		return (OVER);
	if (log_activity(
		session->start_ms, "has taken a dongle", coder, 0) == OVER)
		return (OVER);
	return (WORKING);
}

static t_status	routine(t_coder *coder, t_args *args, t_session *session)
{
	size_t_thread_set(
		&coder->last_compile_mutex, &coder->last_compile, current_time_ms());
	if (log_activity(session->start_ms, "is compiling", coder, args->ttc) == OVER)
		return (OVER);
	pthread_mutex_lock(&session->dongles_mutex);
	coder->left->last_use = current_time_ms();
	coder->right->last_use = current_time_ms();
	coder->left->available = true;
	coder->right->available = true;
	pthread_cond_broadcast(&session->dongles_cond);
	pthread_mutex_unlock(&session->dongles_mutex);
	if (log_activity(
		session->start_ms, "is debugging", coder, args->ttd) == OVER)
		return (OVER);
	if (log_activity(
		session->start_ms, "is refactoring", coder, args->ttr) == OVER)
		return (OVER);
	return (WORKING);
}

extern void	*handle_coder(void *arg)
{
	t_coder				*coder;
	t_status			status;
	size_t				i;

	coder = (t_coder *)arg;
	if (wait_session_start(coder->session) == OVER)
		return (NULL);
	i = -1;
	status = WORKING;
	while (++i < coder->args->nocr)
	{
		status = take_dongles(coder, coder->session);
		if (status != WORKING)
			return (NULL);
		if (routine(coder, coder->args, coder->session) != WORKING)
			return (NULL);
	}
	bool_thread_set(&coder->over_mutex, &coder->over, true);
	return (NULL);
}
