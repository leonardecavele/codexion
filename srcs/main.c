/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:22:50 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 17:47:02 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "args.h"
#include "objects.h"
#include "debug.h"
#include "session.h"

int	main(int ac, char **av)
{
	t_errcode	errcode;
	t_session	session;
	t_args		args;

	args = (t_args){0};
	errcode = parse_args(ac - 1, av + 1, &args);
	if (errcode != NO_ERROR)
		return (error_message(errcode));
	if (args.noc == 0)
		return (error_message(ZERO_CODERS_ERROR));
	session.objects = (t_objects){0};
	errcode = allocate_objects(args, &session.objects);
	if (errcode == NO_ERROR)
	{
		set_up_objects(&args, &session.objects, &session);
		debug_ids(args, session.objects);
		debug_args(args);
		errcode = handle_session(&args, &session);
	}
	destroy_objects(&session.objects);
	return (error_message(errcode));
}
