/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:15:46 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 15:53:04 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitor.h"

static bool	check_burnout(t_objects *objects, t_args *args, t_session *session)
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
			log_activity(
				session->start_ms, "burned out", &objects->coders[i], 0
			);
			pthread_mutex_unlock(&objects->coders[i].last_compile_mutex);
			return BURN;
		}
		pthread_mutex_unlock(&objects->coders[i].last_compile_mutex);
	}
	return CHILL;
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

	objects = (t_objects *)arg;
	args = objects->coders[0].args;
	session = objects->coders[0].session;
	while (!usleep(100) && !session->ready)
		if (session->killed)
			return (NULL);
	while (1)
		if (!usleep(100) && (check_burnout(objects, args, session) == BURN
			|| check_over(objects, args) == true))
		{
			session->killed = true;
			return (NULL);
		}
	return (NULL);
}
