/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 16:30:07 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_H
# define SESSION_H

# include <pthread.h>

# include "args.h"
# include "error.h"
# include "coder.h"
# include "monitor.h"

typedef struct s_objects	t_objects;

typedef struct s_session
{
	size_t			start_ms;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dongles_mutex;
	pthread_cond_t	dongles_cond;
	pthread_t		monitor;
	t_objects		objects;
	bool			ready;
	pthread_mutex_t	killed_mutex;
	bool			killed;
}	t_session;

t_errcode	start_session(t_args *args, t_session *session);
void		wait_session(size_t n_threads, t_session *session);

#endif
