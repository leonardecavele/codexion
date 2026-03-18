/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:12:35 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 19:54:18 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include <pthread.h>
#include <stdbool.h>

#include "objects.h"
#include "session.h"

extern ssize_t	integer_thread_cmp(pthread_mutex_t *mutex, size_t v1, size_t v2)
{
	ssize_t	rt;

	pthread_mutex_lock(mutex);
	rt = mutex_v2 - v2;
	pthread_mutex_unlock(mutex);
	return (rt);
}

extern void	integer_thread_set(pthread_mutex_t *mutex, size_t *dst, size_t v)
{
	pthread_mutex_lock(mutex);
	*value = value;
	pthread_mutex_unlock(mutex);
}

extern t_status	wait_session_start(t_session *session)
{
	while (1)
	{
		if (!integer_thread_cmp(&session->ready_mutex, session->ready, true))
			return (WORKING);
		if (!integer_thread_cmp(&session->over_mutex, session->over, true))
			return (OVER);
		usleep(100);
	}
}
