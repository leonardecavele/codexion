/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:40:19 by ldecavel          #+#    #+#             */
/*   Updated: 2026/04/05 11:26:21 by ldecavel         ###   ########.fr       */
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

static t_dongle *pick_target_dongle(t_coder *coder)
{
	t_dongle *target;

	target = coder->right;
	if (coder->id % 2 == 0)
		target = coder->left;
	return (target);
}

static t_status	try_take_dongles(
	t_coder *coder, t_session *session, t_dongle *target
)
{
	queue_enter(session, coder);
	while (1)
	{
		if (queue_can_take(session, coder))
		{
			coder->left->available = false;
			coder->right->available = false;
			queue_leave(coder);
			break ;
		}
		pthread_mutex_lock(&target->mutex);
		while (!target->available)
			pthread_cond_wait(&target->cond, &target->mutex);
		pthread_mutex_unlock(&target->mutex);
		if (!bool_thread_cmp(&session->over_mutex, &session->over, true))
		{
			queue_leave(coder);
			return (OVER);
		}
	}
	return (WORKING);
}

static t_status	take_dongles(t_coder *coder, t_session *session)
{
	t_dongle	*target;

	target = pick_target_dongle(coder);
	if (try_take_dongles(coder, session, target) == OVER)
		return (OVER);
	if (log_activity(session->start_ms, "has taken a dongle", coder, 0) == OVER)
	{
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
		return (OVER);
	}
	if (log_activity(session->start_ms, "has taken a dongle", coder, 0) == OVER)
	{
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
		return (OVER);
	}
	return (WORKING);
}

static t_status	routine(t_coder *coder, t_args *args, t_session *session)
{
	size_t_thread_set(
		&coder->last_compile_mutex, &coder->last_compile, current_time_ms());
	if (log_activity(
			session->start_ms, "is compiling", coder, args->ttc) == OVER)
	{
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
		return (OVER);
	}
	coder->left->last_use = current_time_ms();
	coder->right->last_use = current_time_ms();
	coder->left->available = true;
	coder->right->available = true;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
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
