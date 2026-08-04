/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 22:32:00 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 20:01:37 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include "objects.h"
#include "args.h"
#include "helpers.h"
#include "queue.h"
#include "threads.h"
#include "debug.h"

static bool	neighbor_can(t_coder *coder)
{
	if (!coder->waiting)
		return (false);
	if (!coder->left->available || !coder->right->available)
		return (false);
	if (elapsed_time_ms(coder->left->last_use) < coder->args->dc)
		return (false);
	if (elapsed_time_ms(coder->right->last_use) < coder->args->dc)
		return (false);
	return (true);
}

static bool	has_priority(t_coder *coder, t_coder *other)
{
	size_t	coder_deadline;
	size_t	other_deadline;
	size_t	coder_last_compile;
	size_t	other_last_compile;

	if (!other->waiting)
		return (true);
	if (coder->args->scheduler == FIFO)
		return (coder->request_seq < other->request_seq);
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
	return (coder->request_seq < other->request_seq);
}

extern void	queue_enter(t_session *session, t_coder *coder)
{
	pthread_mutex_lock(&session->queue_mutex);
	if (!coder->waiting)
	{
		coder->waiting = true;
		coder->request_seq = coder->session->next_request_seq++;
	}
	pthread_mutex_unlock(&session->queue_mutex);
}

extern void	queue_leave(t_session *session, t_coder *coder)
{
	pthread_mutex_lock(&session->queue_mutex);
	coder->waiting = false;
	coder->request_seq = 0;
	pthread_mutex_unlock(&session->queue_mutex);
}

extern bool	queue_can_take(t_session *session, t_coder *coder)
{
	t_coder	*other_left;
	t_coder	*other_right;
	bool	can_take;

	can_take = false;
	pthread_mutex_lock(&session->queue_mutex);
	other_left = coder->left->heap[0];
	if (coder->left->heap[0] == coder)
		other_left = coder->left->heap[1];
	other_right = coder->right->heap[0];
	if (coder->right->heap[0] == coder)
		other_right = coder->right->heap[1];
	if (coder->left->available && coder->right->available
		&& elapsed_time_ms(coder->left->last_use) >= coder->args->dc
		&& elapsed_time_ms(coder->right->last_use) >= coder->args->dc
		&& (has_priority(coder, other_left) || !neighbor_can(other_left))
		&& (has_priority(coder, other_right) || !neighbor_can(other_right)))
	{
		can_take = true;
		debug_priority(*coder->args, coder, other_left, other_right);
	}
	pthread_mutex_unlock(&session->queue_mutex);
	return (can_take);
}
