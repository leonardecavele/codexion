/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 00:57:09 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/20 02:59:40 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUTEX_H
# define MUTEX_H

# include <pthread.h>
# include <stddef.h>

# include "error.h"

t_errcode	init_mutexes(pthread_mutex_t **mutexes, size_t n);
void		destroy_mutexes(pthread_mutex_t **mutexes, size_t n);
void		destroy_coder_mutexes(t_objects *objs, size_t n);

#endif
