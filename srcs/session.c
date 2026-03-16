/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:43 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 18:33:36 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "session.h"
#include "coder.h"

extern t_errcode	start_session(t_args *args, t_objects *objects)
{
	size_t		i;
	t_session	session;

	i = -1;
	session.start_ms = current_time_ms();
	while (++i < args->noc)
	{
		&objects->coders[i].session = &session;
		if (pthread_create(
				&objects->coders[i].thread, NULL,
				handle_coder, &objects->coders[i]
			) != 0)
		{
			wait_session(objects, i);
			return (THREAD_CREATE_ERROR);
		}
	}
	return (NO_ERROR);
}

extern void	wait_session(t_objects *objects, size_t n_threads)
{
	size_t	i;

	i = -1;
	while (++i < n_threads)
		pthread_join(objects->coders[i].thread, NULL);
}
