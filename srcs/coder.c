/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 17:40:19 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 18:55:09 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

// est-ce que je peux prendre tel dongle
// -> verifie la queue

static void	log_activity(size_t start_ms, size_t coder_id, char *activity)
{
	fprintf(
		stdout, "%zu %d %s\n", elapsed_time_ms(start_ms), coder_id, activity
	);
}

static void	take_dongles(
	size_t coder_id, pthread_mutex_t *dongle_1, pthread_mutex_t *dongle_2
)
{
	pthread_mutex_lock(dongle_1);
	log_activity(coder->session.start_ms, "has taken a dongle", coder->id);
	pthread_mutex_lock(dongle_2);
	log_activity(coder->session.start_ms, "has taken a dongle", coder->id);
}

extern void	*handle_coder(void *arg)
{
	t_coder	*coder;
	size_t	start_ms;

	coder = (t_coder *)arg;
	start_ms = coder->session.start_ms;
	if (coder->id % 2 == 0)
		take_dongles(&coder->left->mutex, &coder->right->mutex);
	else
		take_dongles(&coder->right->mutex, &coder->left->mutex);

	log_activity(coder->session.start_ms, "is compiling", coder->id);

	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);

	log_activity(coder->session.start_ms, "is debugging", coder->id);

	return (NULL);
}
