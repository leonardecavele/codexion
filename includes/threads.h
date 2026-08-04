/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:13:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 20:11:32 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_session	t_session;
typedef enum e_status		t_status;

t_status	wait_session_start(t_session *session);

ssize_t		bool_thread_cmp(pthread_mutex_t *mutex, bool *v1, bool v2);
ssize_t		size_t_thread_cmp(pthread_mutex_t *mutex, size_t *v1, size_t v2);
void		bool_thread_set(pthread_mutex_t *mutex, bool *dst, bool v);
void		size_t_thread_set(pthread_mutex_t *mutex, size_t *dst, size_t v);

#endif
