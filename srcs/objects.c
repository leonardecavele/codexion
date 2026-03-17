/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:09:00 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/17 12:42:25 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

extern void	set_up_objects(t_args args, t_objects *objects)
{
	size_t	i;

	i = -1;
	while (++i < args.noc)
	{
		objects->coders[i].left = &objects->dongles[
			(i + args.noc - 1) % args.noc
		];
		objects->coders[i].right = &objects->dongles[i % args.noc];
		objects->dongles[i].id = i + 1;
		objects->coders[i].id = i + 1;
		objects->coders[i].args = &args;
	}
	return ;
}

extern t_errcode	allocate_objects(t_args args, t_objects *objects)
{
	objects->coders = malloc(sizeof(t_coder) * args.noc);
	if (!objects->coders)
		return (MALLOC_ERROR);
	objects->dongles = malloc(sizeof(t_dongle) * args.noc);
	if (!objects->dongles)
		return (MALLOC_ERROR);
	return (NO_ERROR);
}

extern void	destroy_objects(t_objects *objects)
{
	free(objects->coders);
	free(objects->dongles);
	return ;
}
