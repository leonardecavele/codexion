/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:15:46 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 14:16:49 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitor.h"

static bool	check_coders(t_objects *objects, t_args *args, t_session *session)
{
	size_t	i;

	i = -1;
	while (++i < args->noc)
	{
		pthread_mutex_lock(&objects->coders[i].last_compile_mutex);
		if (current_time_ms() - objects->coders[i].last_compile >= args->ttb)
		{
			pthread_mutex_lock(&session->print_mutex);
			fprintf(
				stdout, "%zu %zu burned out\n",
				elapsed_time_ms(session->start_ms), objects->coders[i].id
				);
			pthread_mutex_unlock(&session->print_mutex);
			pthread_mutex_unlock(&objects->coders[i].last_compile_mutex);
			return BURN;
		}
		pthread_mutex_unlock(&objects->coders[i].last_compile_mutex);
	}
	return CHILL;
}

extern void	*handle_monitor(void *arg)
{
	t_objects	*objects;
	t_args		*args;
	t_session	*session;

	objects = (t_objects *)arg;
	args = objects->coders[0].args;
	session = objects->coders[0].session;
	while (!session->ready && usleep(100))
		if (session->killed)
			return (NULL);
	while (1)
	{
		if (check_coders(objects, args, session) == BURN)
		{
			session->killed = true;
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
