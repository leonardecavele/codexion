/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:18:32 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 21:55:51 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_H
# define HELPERS_H

# include <stddef.h>
# include <pthread.h>

# include "session.h"

typedef struct s_coder	t_coder;

size_t		current_time_ms(void);
size_t		elapsed_time_ms(size_t start_ms);

t_status	log_activity(
				size_t start_ms, char *activity,
				t_coder *coder, size_t time_to_wait
				);

void		log_burnout(size_t start_ms, t_coder *coder);

#endif
