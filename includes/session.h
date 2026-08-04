/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 19:14:48 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_H
# define SESSION_H

# include <stdbool.h>

# include "error.h"

typedef struct s_objects	t_objects;
typedef struct s_args		t_args;

typedef enum e_status
{
	WORKING,
	OVER
}	t_status;

typedef struct s_session
{
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dongles_mutex;
	pthread_mutex_t	over_mutex;
	pthread_mutex_t	ready_mutex;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	dongles_cond;
	pthread_t		monitor;
	size_t			start_ms;
	size_t			next_request_seq;
	bool			over;
	bool			ready;
	t_objects		objects;
	t_args			*args;
}	t_session;

t_errcode	handle_session(t_args *args, t_session *session);

#endif
