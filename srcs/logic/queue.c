/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 22:32:00 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 22:47:05 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include "objects.h"
#include "args.h"
#include "helpers.h"
#include "queue.h"

static t_coder	*get_other_coder(t_dongle *dongle, t_coder *coder)
{
	if (dongle->heap[0] == coder)
		return (dongle->heap[1]);
	return (dongle->heap[0]);
}

static bool	neighbor_can_take_now(t_coder *coder)
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

void	queue_enter(t_coder *coder)
{
	if (coder->waiting)
		return ;
	coder->waiting = true;
	coder->request_seq = coder->session->next_request_seq;
	coder->session->next_request_seq++;
}

void	queue_leave(t_coder *coder)
{
	coder->waiting = false;
	coder->request_seq = 0;
}

bool	queue_can_take(t_coder *coder)
{
	t_coder	*other_left;
	t_coder	*other_right;

	if (!coder->left->available || !coder->right->available)
		return (false);
	if (elapsed_time_ms(coder->left->last_use) < coder->args->dc)
		return (false);
	if (elapsed_time_ms(coder->right->last_use) < coder->args->dc)
		return (false);
	other_left = get_other_coder(coder->left, coder);
	other_right = get_other_coder(coder->right, coder);
	if (!has_priority(coder, other_left) && neighbor_can_take_now(other_left))
		return (false);
	if (!has_priority(coder, other_right) && neighbor_can_take_now(other_right))
		return (false);
	return (true);
}
