/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:40:19 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/17 13:58:35 by ldecavel         ###   ########.fr       */
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
	t_coder *coder, pthread_mutex_t *dongle_1, pthread_mutex_t *dongle_2
)
{
	pthread_mutex_lock(dongle_1);
	exec_activity(coder->session->start_ms, "has taken a dongle", coder, 0);
	pthread_mutex_lock(dongle_2);
	exec_activity(coder->session->start_ms, "has taken a dongle", coder, 0);
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
			take_dongles(coder, &coder->left->mutex, &coder->right->mutex);
		else
			take_dongles(coder, &coder->right->mutex, &coder->left->mutex);
		exec_activity(start_ms, "is compiling", coder, coder->args->ttc);
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
		exec_activity(start_ms, "is debugging", coder, coder->args->ttd);
		exec_activity(start_ms, "is refactoring", coder, coder->args->ttr);
	}
	return (NULL);
}
