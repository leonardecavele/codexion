/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 20:00:59 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_H
# define SESSION_H

# include <pthread.h>

# include "args.h"
# include "error.h"
# include "coder.h"

typedef struct s_objects	t_objects;

typedef struct s_session
{
	size_t			start_ms;
	pthread_mutex_t	print_mutex;
}	t_session;

t_errcode	start_session(t_args *args, t_objects *objects, t_session *session);
void		wait_session(t_objects *objects, size_t n_threads);

#endif
