/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:15:46 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 20:36:49 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdbool.h>

#include "objects.h"
#include "session.h"
#include "helpers.h"
#include "args.h"
#include "threads.h"

static ssize_t	check_burnout(t_objects *objects, t_args *args)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
	{
		if (!bool_thread_cmp(
			&objects->coders[i].over_mutex, &objects->coders[i].over, true))
			continue ;
		if (size_t_thread_cmp(
			&objects->coders[i].last_compile_mutex,
			&objects->coders[i].last_compile,
			current_time_ms() - args->ttb
		) <= 0)
		return (objects->coders[i].id);
	}
	return (-1);
}

static bool	check_over(t_objects *objects, t_args *args)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
		if (!bool_thread_cmp(
			&objects->coders[i].over_mutex, &objects->coders[i].over, false))
			return (false);
	return (true);
}

extern void	*handle_monitor(void *arg)
{
	t_objects	*objects;
	t_args		*args;
	t_session	*session;
	int			burnout_id;

	objects = (t_objects *)arg;
	args = objects->coders[0].args;
	session = objects->coders[0].session;
	if (wait_session_start(session) == OVER)
		return (NULL);
	// retravailler check burnout
	while (1)
	{
		burnout_id = check_burnout(objects, args);
		if (burnout_id > -1)
		{
			bool_thread_set(&session->over_mutex, &session->over, true);
			pthread_mutex_lock(&session->dongles_mutex);
			pthread_cond_broadcast(&session->dongles_cond);
			pthread_mutex_unlock(&session->dongles_mutex);
			log_activity(
				session->start_ms, "burned out", &objects->coders[burnout_id], 0
			);
			return (NULL);
		}
		if (check_over(objects, args))
		{
			bool_thread_set(&session->over_mutex, &session->over, true);
			pthread_mutex_lock(&session->dongles_mutex);
			pthread_cond_broadcast(&session->dongles_cond);
			pthread_mutex_unlock(&session->dongles_mutex);
			return (NULL);
		}
	}
	return (NULL);
}
