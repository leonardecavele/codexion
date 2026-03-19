/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 22:33:49 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 22:40:03 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

# include <stdbool.h>

typedef struct s_coder	t_coder;

void	queue_enter(t_coder *coder);
void	queue_leave(t_coder *coder);
bool	queue_can_take(t_coder *coder);

#endif
