/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 17:55:00 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_H
# define SESSION_H

# include <stdbool.h>

# include "error.h"

typedef struct s_objects	t_objects;
typedef struct s_args		t_args;

typedef struct s_session
{
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dongles_mutex;
	pthread_mutex_t	killed_mutex;
	pthread_cond_t	dongles_cond;
	pthread_t		monitor;
	size_t			start_ms;
	bool			ready;
	bool			killed;
	t_objects		objects;
}	t_session;

t_errcode	handle_session(t_args *args, t_session *session);

#endif
