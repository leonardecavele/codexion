/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:58:44 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 16:01:22 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "args.h"
#include "objects.h"

extern void	debug_ids(t_args args, t_objects objects)
{
	size_t	i;

	if (!args.debug)
		return ;
	fprintf(stdout, "[DEBUG IDS]\n");
	i = -1;
	while (++i < args.noc)
	{
		fprintf(
			stdout,
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
	fprintf(stdout, "%s\n", s);
}

extern void	debug_args(t_args args)
{
	if (!args.debug)
		return ;
	fprintf(stdout, "[DEBUG ARGS]\n");
	fprintf(stdout, "number of coders [%zu]\n", args.noc);
	fprintf(stdout, "time to burnout [%zu]\n", args.ttb);
	fprintf(stdout, "time to compile [%zu]\n", args.ttc);
	fprintf(stdout, "time to debug [%zu]\n", args.ttd);
	fprintf(stdout, "time to refactor [%zu]\n", args.ttr);
	fprintf(stdout, "number of compiles required [%zu]\n", args.nocr);
	fprintf(stdout, "dongle cooldown [%zu]\n", args.dc);
	if (args.scheduler == FIFO)
		fprintf(stdout, "scheduler [FIFO]\n");
	else
		fprintf(stdout, "scheduler [EDF]\n");
}
