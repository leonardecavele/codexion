/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:15:46 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 19:56:08 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdbool.h>

#include "objects.h"
#include "session.h"
#include "helpers.h"
#include "args.h"
#include "threads.h"

static t_status	check_burnout(t_objects *objects, t_args *args)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
	{
		if (!integer_thread_cmp(
			&objects->coders[i].over_mutex, objects->coders[i].over, true)
			continue ;
		if (integer_thread_cmp(
				&objects->coders[i].last_compile_mutex,
				current_time_ms() - objects->coders[i].last_compile, args->ttb
			) >= 0)
			return (objects->coders[i].id);
	}
	return (-1);
}

static bool	check_over(t_objects *objects, t_args *args)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
		if (!integer_thread_cmp(
			&objects->coders[i].over_mutex, objects->coders[i].over, false)
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
	while (1)
	{
		burnout_id = check_burnout(objects, args);
		if (!usleep(100) && (burnout_id > -1
			|| check_over(objects, args) == true))
		{
			pthread_mutex_lock(&session->dongles_mutex);
			integer_thread_set(&session->over_mutex, &session->over, true);
			pthread_cond_broadcast(&session->dongles_cond);
			pthread_mutex_unlock(&session->dongles_mutex);
			log_activity(
				session->start_ms, "burned out", &objects->coders[burnout_id], 0
			);
			return (NULL);
		}
	}
	return (NULL);
}
