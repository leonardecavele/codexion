/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 12:21:53 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 18:23:57 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

void	debug_ids(t_args args, t_objects objects);
void	debug_args(t_args args);
void	debug_print(t_args args, const char *s);
void	debug_priority(
			t_args args,
			t_coder *coder,
			t_coder *other_left,
			t_coder *other_right
			);
#endif
