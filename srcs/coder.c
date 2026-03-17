/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:40:19 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/17 18:59:32 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "session.h"

// est-ce que je peux prendre tel dongle
// -> verifie la queue

static void	exec_activity(
	size_t start_ms, char *activity, t_coder *coder, size_t time_to_wait
)
{
	pthread_mutex_lock(&coder->session->print_mutex);
	fprintf(
		stdout, "%zu %zu %s\n", elapsed_time_ms(start_ms), coder->id, activity
		);
	pthread_mutex_unlock(&coder->session->print_mutex);
	usleep(time_to_wait * 1000);
}

static void	take_dongles(
	t_coder *coder, t_dongle *dongle_1, t_dongle *dongle_2
)
{
	// check last time
	// check dongle->last_use
	while (
		!((elapsed_time_ms(dongle_1->last_use) >= coder->args->dc
		 && elapsed_time_ms(dongle_2->last_use) >= coder->args->dc))
	)
		usleep(100);
	pthread_mutex_lock(&coder->session->dongles_mutex);
	while (!(dongle_1->available && dongle_2->available))
		pthread_cond_wait(
			&coder->session->dongles_cond, &coder->session->dongles_mutex
		);
	dongle_1->available = false;
	dongle_2->available = false;
	exec_activity(coder->session->start_ms, "has taken a dongle", coder, 0);
	exec_activity(coder->session->start_ms, "has taken a dongle", coder, 0);
	pthread_mutex_unlock(&coder->session->dongles_mutex);
}

extern void	*handle_coder(void *arg)
{
	t_coder	*coder;
	size_t	start_ms;
	size_t	i;

	i = -1;
	coder = (t_coder *)arg;
	start_ms = coder->session->start_ms;
	while (++i < coder->args->nocr)
	{
		if (coder->id % 2 == 0)
			take_dongles(coder, coder->left, coder->right);
		else
			take_dongles(coder, coder->right, coder->left);
		exec_activity(start_ms, "is compiling", coder, coder->args->ttc);
		// coder last compile
		coder->left->last_use = current_time_ms();
		coder->right->last_use = current_time_ms();

		pthread_mutex_lock(&coder->session->dongles_mutex);
		coder->left->available = true;
		coder->right->available = true;
		pthread_cond_broadcast(&coder->session->dongles_cond);
		pthread_mutex_unlock(&coder->session->dongles_mutex);

		exec_activity(start_ms, "is debugging", coder, coder->args->ttd);
		exec_activity(start_ms, "is refactoring", coder, coder->args->ttr);
	}
	return (NULL);
}
