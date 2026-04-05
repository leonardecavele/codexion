/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 22:32:00 by ldecavel          #+#    #+#             */
/*   Updated: 2026/04/05 20:32:13 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include "objects.h"
#include "args.h"
#include "helpers.h"
#include "queue.h"
#include "threads.h"
#include "debug.h"

static t_coder	*other_coder(t_dongle *dongle, t_coder *coder)
{
	if (dongle->heap[0] == coder)
		return (dongle->heap[1]);
	return (dongle->heap[0]);
}

static bool	neighbor_can(t_coder *neighbor, t_dongle *not_shared)
{
	bool can;

	can = true;
	if (!bool_thread_cmp(&neighbor->waiting_mutex, &neighbor->waiting, true))
		return (false);
	// missing one mutex
	pthread_mutex_lock(&not_shared->mutex);
	if (
		(!neighbor->left->available || !neighbor->right->available)
		|| elapsed_time_ms(neighbor->left->last_use) < neighbor->args->dc
		|| elapsed_time_ms(neighbor->right->last_use) < neighbor->args->dc
	)
		can = false;
	pthread_mutex_unlock(&not_shared->mutex);
	return (can);
}

static bool	has_priority(t_coder *coder, t_coder *other)
{
	size_t	coder_deadline;
	size_t	other_deadline;
	size_t	coder_ticket;
	size_t	other_ticket;
	size_t	coder_last_compile;
	size_t	other_last_compile;

	if (!bool_thread_cmp(&other->waiting_mutex, &other->waiting, true))
		return (true);
	pthread_mutex_lock(&coder->ticket_mutex);
	coder_ticket = coder->ticket;
	pthread_mutex_unlock(&coder->ticket_mutex);
	pthread_mutex_lock(&other->ticket_mutex);
	other_ticket = other->ticket;
	pthread_mutex_unlock(&other->ticket_mutex);
	if (coder->args->scheduler == FIFO)
		return (coder_ticket < other_ticket);
	pthread_mutex_lock(&coder->last_compile_mutex);
	coder_last_compile = coder->last_compile;
	pthread_mutex_unlock(&coder->last_compile_mutex);
	pthread_mutex_lock(&other->last_compile_mutex);
	other_last_compile = other->last_compile;
	pthread_mutex_unlock(&other->last_compile_mutex);
	coder_deadline = coder_last_compile + coder->args->ttb;
	other_deadline = other_last_compile + other->args->ttb;
	if (coder_deadline != other_deadline)
		return (coder_deadline < other_deadline);
	return (coder_ticket < other_ticket);
}

extern void	queue_enter(t_session *session, t_coder *coder)
{
	pthread_mutex_lock(&coder->waiting_mutex);
	if (!coder->waiting)
	{
		coder->waiting = true;
		pthread_mutex_lock(&session->next_ticket_mutex);
		size_t_thread_set(
			&coder->ticket_mutex, &coder->ticket, coder->session->next_ticket++
		);
		pthread_mutex_unlock(&session->next_ticket_mutex);
	}
	pthread_mutex_unlock(&coder->waiting_mutex);
}

extern void	queue_leave(t_coder *coder)
{
	bool_thread_set(&coder->waiting_mutex, &coder->waiting, false);
	size_t_thread_set(&coder->ticket_mutex, &coder->ticket, 0);
}




static struct timespec	get_deadline_ms(size_t delay_ms)
{
	struct timespec	deadline;

	clock_gettime(CLOCK_REALTIME, &deadline);
	deadline.tv_sec += delay_ms / 1000;
	deadline.tv_nsec += (long)(delay_ms % 1000) * 1000000L;
	if (deadline.tv_nsec >= 1000000000L)
	{
		deadline.tv_sec += 1;
		deadline.tv_nsec -= 1000000000L;
	}
	return (deadline);
}

#include "errno.h"

extern bool	queue_can_take(t_session *session, t_coder *coder)
{
	t_coder	*other_left;
	t_coder	*other_right;

	(void)session;
	pthread_mutex_lock(&coder->left->mutex);
	pthread_mutex_lock(&coder->right->mutex);
	other_left = other_coder(coder->left, coder);
	other_right = other_coder(coder->right, coder);
	if (!(
		coder->left->available && coder->right->available
		&& (has_priority(coder, other_left)
			|| !neighbor_can(other_left, other_left->left))
		&& (has_priority(coder, other_right)
			|| !neighbor_can(other_right, other_right->right))
	))
	{
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
		return (false);
	}

	if (!(
		elapsed_time_ms(coder->left->last_use) >= coder->args->dc
		&& elapsed_time_ms(coder->right->last_use) >= coder->args->dc
	))
	{
		size_t max;
		size_t left = elapsed_time_ms(coder->left->last_use) - coder->args->dc;
		size_t right = elapsed_time_ms(coder->right->last_use) - coder->args->dc;
		max = right;
		if (right < left)
			max = left;
		struct timespec	time_to_wait = get_deadline_ms(max);

		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
		pthread_mutex_lock(&session->over_mutex);
		if (
			pthread_cond_timedwait(
				&session->over_cond, &session->over_mutex, &time_to_wait
			) == ETIMEDOUT
		)
		{
			pthread_mutex_unlock(&session->over_mutex);
			return (WORKING);
		}
		pthread_mutex_unlock(&session->over_mutex);
	}
	debug_priority(*coder->args, coder, other_left, other_right);
	return (true);
}
