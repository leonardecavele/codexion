/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:18:32 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 17:43:20 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_H
# define HELPERS_H

# include <stddef.h>

# include "monitor.h"

typedef struct s_coder	t_coder;

size_t	current_time_ms(void);
size_t	elapsed_time_ms(size_t start_ms);
t_burnout_status	log_activity(
	size_t start_ms, char *activity, t_coder *coder, size_t time_to_wait
);

#endif
