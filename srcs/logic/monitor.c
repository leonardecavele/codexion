/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:15:46 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 21:08:49 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

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
				&objects->coders[i].over_mutex,
				&objects->coders[i].over,
				true
			))
			continue ;
		if (size_t_thread_cmp(
				&objects->coders[i].last_compile_mutex,
				&objects->coders[i].last_compile,
				current_time_ms() - args->ttb
			) <= 0)
			return (i);
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
	ssize_t		burnout_index;

	objects = (t_objects *)arg;
	args = objects->coders[0].args;
	session = objects->coders[0].session;
	if (wait_session_start(session) == OVER)
		return (NULL);
	while (1)
	{
		burnout_index = check_burnout(objects, args);
		if (burnout_index >= 0)
		{
			pthread_mutex_lock(&session->print_mutex);
			fprintf(stdout, "%zu %zu burned out\n",
				elapsed_time_ms(session->start_ms),
				objects->coders[burnout_index].id);
			pthread_mutex_unlock(&session->print_mutex);
			bool_thread_set(&session->over_mutex, &session->over, true);
			pthread_mutex_lock(&session->dongles_mutex);
			pthread_cond_broadcast(&session->dongles_cond);
			pthread_mutex_unlock(&session->dongles_mutex);
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
		usleep(100);
	}
}
