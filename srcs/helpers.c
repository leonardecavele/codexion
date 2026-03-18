/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:15:59 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 18:05:27 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#include "objects.h"
#include "session.h"
#include "monitor.h"

extern size_t	current_time_ms(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((size_t)timeval.tv_sec * 1000 + (size_t)timeval.tv_usec / 1000);
}

extern size_t	elapsed_time_ms(size_t start_ms)
{
	return ((size_t)(current_time_ms() - start_ms));
}

extern t_burnout_status	log_activity(
	size_t start_ms, char *activity, t_coder *coder, size_t time_to_wait
)
{
	size_t	elapsed;
	size_t	chunk;

	pthread_mutex_lock(&coder->session->print_mutex);
	printf("%zu %zu %s\n", elapsed_time_ms(start_ms), coder->id, activity);
	pthread_mutex_unlock(&coder->session->print_mutex);
	elapsed = 0;
	while (elapsed < time_to_wait)
	{
		pthread_mutex_lock(&coder->session->killed_mutex);
		if (coder->session->killed)
		{
			pthread_mutex_unlock(&coder->session->killed_mutex);
			return (OVER);
		}
		pthread_mutex_unlock(&coder->session->killed_mutex);
		chunk = time_to_wait - elapsed;
		if (chunk > 1)
			chunk = 1;
		usleep(chunk * 1000);
		elapsed += chunk;
	}
	return (WORKING);
}
