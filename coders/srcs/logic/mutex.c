/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 00:56:26 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/20 01:27:54 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stddef.h>

#include "error.h"

static void	destroy_mutexes(pthread_mutex_t **mutexes, size_t n)
{
	while (n-- > 0)
		pthread_mutex_destroy(mutexes[n]);
}

extern t_errcode	init_mutexes(pthread_mutex_t **mutexes, size_t n)
{
	size_t	i;

	i = -1;
	while (++i < n)
	{
		if (pthread_mutex_init(mutexes[i], NULL) != 0)
		{
			destroy_mutexes(mutexes, i);
			return (MUTEX_INIT_ERROR);
		}
	}
	return (NO_ERROR);
}
