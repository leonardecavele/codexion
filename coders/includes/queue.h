/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 22:33:49 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 19:20:25 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

# include <stdbool.h>

typedef struct s_coder		t_coder;
typedef struct s_session	t_session;

void	queue_enter(t_session *session, t_coder *coder);
void	queue_leave(t_session *session, t_coder *coder);
bool	queue_can_take(t_session *session, t_coder *coder);

#endif
