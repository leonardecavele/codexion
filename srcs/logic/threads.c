/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:12:35 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 20:13:17 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include "objects.h"
#include "session.h"

extern ssize_t	bool_thread_cmp(pthread_mutex_t *mutex, bool *v1, bool v2)
{
	ssize_t	rt;

	pthread_mutex_lock(mutex);
	rt = *v1 - v2;
	pthread_mutex_unlock(mutex);
	return (rt);
}

extern ssize_t	size_t_thread_cmp(pthread_mutex_t *mutex, size_t *v1, size_t v2)
{
	ssize_t	rt;

	pthread_mutex_lock(mutex);
	rt = *v1 - v2;
	pthread_mutex_unlock(mutex);
	return (rt);
}

extern void	bool_thread_set(pthread_mutex_t *mutex, bool *dst, bool v)
{
	pthread_mutex_lock(mutex);
	*dst = v;
	pthread_mutex_unlock(mutex);
}

extern void	size_t_thread_set(pthread_mutex_t *mutex, size_t *dst, size_t v)
{
	pthread_mutex_lock(mutex);
	*dst = v;
	pthread_mutex_unlock(mutex);
}

extern t_status	wait_session_start(t_session *session)
{
	while (1)
	{
		if (!bool_thread_cmp(&session->ready_mutex, &session->ready, true))
			return (WORKING);
		if (!bool_thread_cmp(&session->over_mutex, &session->over, true))
			return (OVER);
		usleep(100);
	}
}
