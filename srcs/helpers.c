/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:15:59 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 16:46:20 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.h"
#include "session.h"

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
	fprintf(
		stdout, "%zu %zu %s\n", elapsed_time_ms(start_ms), coder->id, activity
		);
	pthread_mutex_unlock(&coder->session->print_mutex);
	elapsed = 0;
	chunk = 1;
	while (elapsed < time_to_wait - elapsed < chunk)
	{
		pthread_mutex_lock(&coder->session->killed_mutex);
		if (coder->session->killed)
			return (OVER);
		pthread_mutex_unlock(&coder->session->killed_mutex);
		if (time_to_wait - elapsed < chunk)
			chunk = time_to_wait - elapsed;
		usleep(chunk * 1000);
		elapsed += chunk;
	}
	return (WORKING);
}
