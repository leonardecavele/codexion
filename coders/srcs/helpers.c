/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:15:59 by ldecavel          #+#    #+#             */
/*   Updated: 2026/04/05 20:06:41 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

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
	if (session->over == false)
	{
		session->over = true;
		printf("%zu %zu burned out\n", elapsed_time_ms(start_ms), coder->id);
	}
	pthread_mutex_unlock(&session->over_mutex);
	pthread_mutex_unlock(&session->print_mutex);
}

static struct timespec	get_deadline_ms(size_t delay_ms)
{
	struct timespec	deadline;

	clock_gettime(CLOCK_REALTIME, &deadline);
	deadline.tv_sec += delay_ms / 1000;
	deadline.tv_nsec += (long)(delay_ms % 1000) * 1000000L;
	if (deadline.tv_nsec >= 1000000000L)
	{
		deadline.tv_sec += 1;
		deadline.tv_nsec -= 1000000000L;
	}
	return (deadline);
}

extern t_status	log_activity(
	size_t start_ms, char *activity, t_coder *coder, size_t time_to_wait
)
{
	t_session			*session;
	struct timespec		deadline;

	session = coder->session;
	pthread_mutex_lock(&session->print_mutex);
	if (!bool_thread_cmp(&session->over_mutex, &session->over, true))
	{
		pthread_mutex_unlock(&session->print_mutex);
		return (OVER);
	}
	printf("%zu %zu %s\n", elapsed_time_ms(start_ms), coder->id, activity);
	deadline = get_deadline_ms(time_to_wait);
	pthread_mutex_unlock(&session->print_mutex);
	pthread_mutex_lock(&session->over_mutex);
	if (
		pthread_cond_timedwait(
			&session->over_cond, &session->over_mutex, &deadline
		) == ETIMEDOUT
	)
	{
		pthread_mutex_unlock(&session->over_mutex);
		return (WORKING);
	}
	pthread_mutex_unlock(&session->over_mutex);
	return (OVER);
}
