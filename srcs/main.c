/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:22:50 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 14:51:27 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "args.h"
#include "objects.h"
#include "debug.h"

#include <pthread.h>

static void	start_session(t_args *args, t_objects *objects)
{
	//size_t	i;

	//i = -1;
	//while (++i < args.noc)
	//	pthread_create(objects->coder[i].thread, NULL, PLACE_HOLDER_FUNC);
	(void)args;
	(void)objects;
}

int	main(int ac, char **av)
{
	t_errcode	errcode;
	t_args		args;
	t_objects	objects;

	args = (t_args){0};
	errcode = parse_args(ac - 1, av + 1, &args);
	if (errcode != NO_ERROR)
		return (error_message(errcode));
	if (args.noc == 0)
		return (error_message(INVALID_ARG_ERROR));
	errcode = allocate_objects(args, &objects);
	if (errcode == NO_ERROR)
	{
		set_up_objects(args, &objects);
		debug_ids(args, objects);
		debug_args(args);
		start_session(&args, &objects);
	}
	destroy_objects(&objects);
	return (error_message(errcode));
}
