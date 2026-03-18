/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:19:06 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 16:55:07 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H

# include "objects.h"

typedef enum e_burnout_status
{
	WORKING = false,
	OVER = true
}	t_burnout_status;

# include "helpers.h"
# include "session.h"

void	*handle_monitor(void *arg);

#endif
