/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:58:44 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 12:01:50 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

extern void	debug_ids(t_args args, t_objects objects)
{
	size_t	i;

	i = -1;
	while (++i < args.noc)
	{
		fprintf(
			stdout,
			"coder id [%zu], left dongle id [%zu], right dongle id [%zu]\n",
			i, objects.coders[i].left->id, objects.coders[i].right->id
		);
	}
}
