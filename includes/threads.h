/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:13:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 19:53:33 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

#include <pthread.h>

typedef struct s_session	t_session;

t_status	wait_session_start(t_session *session);
ssize_t		integer_thread_cmp(pthread_mutex_t *mutex, size_t v1, size_t v2);
void		integer_thread_set(pthread_mutex_t *mutex, size_t *dst, size_t v);

#endif
