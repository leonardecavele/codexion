/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:19:06 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 11:48:40 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H

# include "objects.h"
# include "helpers.h"
# include "session.h"

typedef enum e_burnout_status
{
	CHILL = false,
	BURN = true
}	t_burnout_status;

void	*handle_monitor(void *arg);

#endif
