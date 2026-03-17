/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:22:50 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/17 14:29:27 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "args.h"
#include "objects.h"
#include "debug.h"
#include "session.h"
#include "helpers.h"

int	main(int ac, char **av)
{
	t_session	session;
	t_errcode	errcode;
	t_args		args;

	args = (t_args){0};
	errcode = parse_args(ac - 1, av + 1, &args);
	if (errcode != NO_ERROR)
		return (error_message(errcode));
	if (args.noc == 0)
		return (error_message(INVALID_ARG_ERROR));
	errcode = allocate_objects(args, &session.objects);
	if (errcode == NO_ERROR)
	{
		set_up_objects(&args, &session.objects, &session);
		debug_ids(args, session.objects);
		debug_args(args);
		session.start_ms = current_time_ms();
		errcode = start_session(&args, &session);
		if (errcode == NO_ERROR)
			wait_session(args.noc, &session);
	}
	destroy_objects(&session.objects);
	return (error_message(errcode));
}
