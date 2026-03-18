/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:40:19 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 20:38:31 by ldecavel         ###   ########.fr       */
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



// est-ce que je peux prendre tel dongle
// -> verifie la queue

static t_status	take_dongles(
	t_coder *coder, t_dongle *dongle_1, t_dongle *dongle_2, t_session *session
)
{
	while (1)
	{
		pthread_mutex_lock(&session->dongles_mutex);
		while (!(dongle_1->available && dongle_2->available))
		{
			pthread_mutex_lock(&session->over_mutex);
			if (session->over)
			{
				pthread_mutex_unlock(&session->over_mutex);
				pthread_mutex_unlock(&session->dongles_mutex);
				return (OVER);
			}
			pthread_mutex_unlock(&session->over_mutex);
			pthread_cond_wait(&session->dongles_cond, &session->dongles_mutex);
		}
		if (elapsed_time_ms(dongle_1->last_use) >= coder->args->dc
			&& elapsed_time_ms(dongle_2->last_use) >= coder->args->dc)
		{
			dongle_1->available = false;
			dongle_2->available = false;
			pthread_mutex_unlock(&session->dongles_mutex);
			break ;
		}
		pthread_mutex_unlock(&session->dongles_mutex);
		pthread_mutex_lock(&session->over_mutex);
		if (session->over)
		{
			pthread_mutex_unlock(&session->over_mutex);
			return (OVER);
		}
		pthread_mutex_unlock(&session->over_mutex);
		usleep(100);
	}
	if (log_activity(session->start_ms, "has taken a dongle", coder, 0)
		|| log_activity(session->start_ms, "has taken a dongle", coder, 0))
		return (OVER);
	return (WORKING);
}

static t_status	routine(t_coder *coder, t_args *args, t_session *session)
{
	size_t_thread_set(
		&coder->last_compile_mutex, &coder->last_compile, current_time_ms()
	);
	if (log_activity(session->start_ms, "is compiling", coder, args->ttc))
		return (OVER);
	size_t_thread_set(
		&session->dongles_mutex, &coder->left->last_use, current_time_ms());
	size_t_thread_set(
		&session->dongles_mutex, &coder->right->last_use, current_time_ms());
	bool_thread_set(&session->dongles_mutex, &coder->left->available, true);
	bool_thread_set(&session->dongles_mutex, &coder->right->available, true);
	pthread_cond_broadcast(&session->dongles_cond);
	if (log_activity(session->start_ms, "is debugging", coder, args->ttd)
		|| log_activity(session->start_ms, "is refactoring", coder, args->ttr))
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
		if (coder->id % 2 == 0)
			status = take_dongles(
				coder, coder->left, coder->right, coder->session
			);
		else
			status = take_dongles(
				coder, coder->right, coder->left, coder->session
			);
		if (status != WORKING)
			return (NULL);
		if (routine(coder, coder->args, coder->session) != WORKING)
			return (NULL);
	}
	bool_thread_set(&coder->over_mutex, &coder->over, true);
	return (NULL);
}
