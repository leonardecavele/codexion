/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:15:59 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 21:32:38 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#include "objects.h"
#include "session.h"
#include "threads.h"
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

extern void	log_burnout(size_t start_ms, t_coder *coder)
{
	pthread_mutex_lock(&coder->session->print_mutex);
	printf("%zu %zu burned out\n", elapsed_time_ms(start_ms), coder->id);
	pthread_mutex_unlock(&coder->session->print_mutex);
}

extern t_status	log_activity(
	size_t start_ms, char *activity, t_coder *coder, size_t time_to_wait
)
{
	size_t	end_ms;
	size_t	now_ms;
	size_t	remaining_ms;

	if (!bool_thread_cmp(
			&coder->session->over_mutex,
			&coder->session->over,
			true
		))
		return (OVER);
	pthread_mutex_lock(&coder->session->print_mutex);
	printf("%zu %zu %s\n", elapsed_time_ms(start_ms), coder->id, activity);
	pthread_mutex_unlock(&coder->session->print_mutex);
	end_ms = current_time_ms() + time_to_wait;
	while (1)
	{
		if (!bool_thread_cmp(
				&coder->session->over_mutex,
				&coder->session->over,
				true
			))
			return (OVER);
		now_ms = current_time_ms();
		if (now_ms >= end_ms)
			break ;
		remaining_ms = end_ms - now_ms;
		if (remaining_ms > 1)
			remaining_ms = 1;
		usleep(remaining_ms * 1000);
	}
	return (WORKING);
}
