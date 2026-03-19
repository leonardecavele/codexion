/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:58:44 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 18:24:09 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "args.h"
#include "objects.h"
#include "session.h"

extern void	debug_ids(t_args args, t_objects objects)
{
	size_t	i;

	if (!args.debug)
		return ;
	printf("[DEBUG IDS]\n");
	i = -1;
	while (++i < args.noc)
	{
		printf(
			"coder id [%zu], left dongle id [%zu], right dongle id [%zu]\n",
			objects.coders[i].id, objects.coders[i].left->id,
			objects.coders[i].right->id
			);
	}
}

extern void	debug_print(t_args args, const char *s)
{
	if (!args.debug)
		return ;
	printf("%s\n", s);
}

extern void	debug_priority(
	t_args args, t_coder *coder, t_coder *other_left, t_coder *other_right
)
{
	t_session	*session;

	if (!args.debug)
		return ;
	session = coder->session;
	pthread_mutex_lock(&session->print_mutex);
	printf(
		"coder %zu has priority over coders %zu and %zu\n",
		coder->id,
		other_left->id,
		other_right->id
		);
	pthread_mutex_unlock(&session->print_mutex);
}

extern void	debug_args(t_args args)
{
	if (!args.debug)
		return ;
	printf("[DEBUG ARGS]\n");
	printf("number of coders [%zu]\n", args.noc);
	printf("time to burnout [%zu]\n", args.ttb);
	printf("time to compile [%zu]\n", args.ttc);
	printf("time to debug [%zu]\n", args.ttd);
	printf("time to refactor [%zu]\n", args.ttr);
	printf("number of compiles required [%zu]\n", args.nocr);
	printf("dongle cooldown [%zu]\n", args.dc);
	if (args.scheduler == FIFO)
		printf("scheduler [FIFO]\n");
	else
		printf("scheduler [EDF]\n");
}
