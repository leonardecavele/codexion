/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:15:59 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 12:11:09 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#include "objects.h"
#include "session.h"
#include "threads.h"
#include "monitor.h"

extern size_t	current_time_ms(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((size_t)timeval.tv_sec * 1000 + (size_t)timeval.tv_usec / 1000);
}

extern size_t	elapsed_time_ms(size_t start_ms)
{
	return ((size_t)(current_time_ms() - start_ms));
}

extern void	log_burnout(size_t start_ms, t_coder *coder)
{
	t_session	*session;

	session = coder->session;
	pthread_mutex_lock(&session->print_mutex);
	pthread_mutex_lock(&session->over_mutex);
	if (!session->over)
	{
		session->over = true;
		printf("%zu %zu burned out\n", elapsed_time_ms(start_ms), coder->id);
	}
	pthread_mutex_unlock(&session->over_mutex);
	pthread_mutex_unlock(&session->print_mutex);
}

static t_status	vigilant_sleep(t_session *session, size_t time_to_wait)
{
	size_t	end_ms;
	size_t	now_ms;
	size_t	remaining_ms;

	end_ms = current_time_ms() + time_to_wait;
	while (1)
	{
		if (!bool_thread_cmp(&session->over_mutex, &session->over, true))
			return (OVER);
		now_ms = current_time_ms();
		if (now_ms >= end_ms)
			break ;
		remaining_ms = end_ms - now_ms;
		if (remaining_ms > 1)
			remaining_ms = 1;
		usleep(remaining_ms * 1000);
	}
	return (WORKING);
}

extern t_status	log_activity(
	size_t start_ms, char *activity, t_coder *coder, size_t time_to_wait
)
{
	t_session	*session;

	session = coder->session;
	pthread_mutex_lock(&session->print_mutex);
	if (!bool_thread_cmp(&session->over_mutex, &session->over, true))
	{
		pthread_mutex_unlock(&session->print_mutex);
		return (OVER);
	}
	printf("%zu %zu %s\n", elapsed_time_ms(start_ms), coder->id, activity);
	pthread_mutex_unlock(&session->print_mutex);
	return (vigilant_sleep(session, time_to_wait));
}
