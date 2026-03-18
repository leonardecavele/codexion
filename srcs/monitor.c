/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:15:46 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 17:16:01 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitor.h"

static t_burnout_status	check_burnout(t_objects *objects, t_args *args)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
	{
		pthread_mutex_lock(&objects->coders[i].over_mutex);
		if (objects->coders[i].over == true)
		{
			pthread_mutex_unlock(&objects->coders[i].over_mutex);
			continue ;
		}
		pthread_mutex_unlock(&objects->coders[i].over_mutex);
		pthread_mutex_lock(&objects->coders[i].last_compile_mutex);
		if (current_time_ms() - objects->coders[i].last_compile >= args->ttb)
		{
			pthread_mutex_unlock(&objects->coders[i].last_compile_mutex);
			return (objects->coders[i].id);
		}
		pthread_mutex_unlock(&objects->coders[i].last_compile_mutex);
	}
	return (-1);
}

static bool	check_over(t_objects *objects, t_args *args)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
	{
		pthread_mutex_lock(&objects->coders[i].over_mutex);
		if (objects->coders[i].over == false)
		{
			pthread_mutex_unlock(&objects->coders[i].over_mutex);
			return (false);
		}
		pthread_mutex_unlock(&objects->coders[i].over_mutex);
	}
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
	while (!usleep(100) && !session->ready)
	{
		pthread_mutex_lock(&session->killed_mutex);
		if (session->killed)
		{
			pthread_mutex_unlock(&session->killed_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&session->killed_mutex);
	}
	while (1)
	{
		burnout_id = check_burnout(objects, args);
		if (!usleep(100) && (burnout_id > -1
			|| check_over(objects, args) == true))
		{
			pthread_mutex_lock(&session->dongles_mutex);
			pthread_mutex_lock(&session->killed_mutex);
			session->killed = true;
			pthread_mutex_unlock(&session->killed_mutex);
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
