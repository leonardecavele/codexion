/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:43 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/17 13:47:41 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "session.h"
#include "objects.h"

extern t_errcode	start_session(t_args *args, t_session *session)
{
	size_t		i;
	t_objects	*objects;

	i = -1;
	objects = &session->objects;
	if (pthread_create(&session->monitor, NULL, handle_monitor, &objects) != 0)
		return (THREAD_CREATE_ERROR);
	while (++i < args->noc)
	{
		objects->coders[i].session = session;
		if (pthread_create(
				&objects->coders[i].thread, NULL,
				handle_coder, &objects->coders[i]
			) != 0)
		{
			wait_session(i, session);
			return (THREAD_CREATE_ERROR);
		}
	}
	return (NO_ERROR);
}

extern void	wait_session(size_t n_threads, t_session *session)
{
	size_t	i;

	i = -1;
	while (++i < n_threads)
		pthread_join(session->objects.coders[i].thread, NULL);
	pthread_join(session->monitor, NULL);
}
