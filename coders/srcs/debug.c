/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:58:44 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/20 13:37:56 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "args.h"
#include "objects.h"
#include "session.h"
#include "threads.h"

extern void	debug_ids(t_args args, t_objects objects)
{
	size_t	i;

	if (!args.debug)
		return ;
	fprintf(stderr, "[DEBUG IDS]\n");
	i = -1;
	while (++i < args.noc)
	{
		fprintf(stderr,
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
	fprintf(stderr, "[DEBUG LOG]\n");
	fprintf(stderr, "%s\n", s);
}

extern void	debug_priority(
	t_args args, t_coder *coder, t_coder *other_left, t_coder *other_right
)
{
	size_t	last[3];

	if (!args.debug)
		return ;
	pthread_mutex_lock(&coder->last_compile_mutex);
	last[0] = coder->last_compile;
	pthread_mutex_unlock(&coder->last_compile_mutex);
	pthread_mutex_lock(&other_left->last_compile_mutex);
	last[1] = other_left->last_compile;
	pthread_mutex_unlock(&other_left->last_compile_mutex);
	pthread_mutex_lock(&other_right->last_compile_mutex);
	last[2] = other_right->last_compile;
	pthread_mutex_unlock(&other_right->last_compile_mutex);
	pthread_mutex_lock(&coder->session->print_mutex);
	fprintf(stderr, "[DEBUG PRIORITY]\n");
	fprintf(
		stderr,
		"winner [%zu] request [%zu] deadline [%zu]\n"
		"left [%zu] request [%zu] deadline [%zu]\n"
		"right [%zu] request [%zu] deadline [%zu]\n",
		coder->id, coder->request_seq, last[0] + args.ttb,
		other_left->id, other_left->request_seq, last[1] + args.ttb,
		other_right->id, other_right->request_seq, last[2] + args.ttb
		);
	pthread_mutex_unlock(&coder->session->print_mutex);
}

extern void	debug_args(t_args args)
{
	if (!args.debug)
		return ;
	fprintf(stderr, "[DEBUG ARGS]\n");
	fprintf(stderr, "number of coders [%zu]\n", args.noc);
	fprintf(stderr, "time to burnout [%zu]\n", args.ttb);
	fprintf(stderr, "time to compile [%zu]\n", args.ttc);
	fprintf(stderr, "time to debug [%zu]\n", args.ttd);
	fprintf(stderr, "time to refactor [%zu]\n", args.ttr);
	fprintf(stderr, "number of compiles required [%zu]\n", args.nocr);
	fprintf(stderr, "dongle cooldown [%zu]\n", args.dc);
	if (args.scheduler == FIFO)
		fprintf(stderr, "scheduler [fifo]\n");
	else
		fprintf(stderr, "scheduler [edf]\n");
}
