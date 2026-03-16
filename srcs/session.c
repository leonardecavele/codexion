/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:43 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 20:04:44 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "session.h"
#include "objects.h"

extern t_errcode	start_session(
	t_args *args, t_objects *objects, t_session *session
)
{
	size_t		i;

	i = -1;
	while (++i < args->noc)
	{
		objects->coders[i].session = session;
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
