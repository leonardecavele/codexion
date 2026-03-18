/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 13:19:06 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 17:41:11 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H

typedef enum e_burnout_status
{
	WORKING,
	OVER
}	t_burnout_status;

void	*handle_monitor(void *arg);

#endif
