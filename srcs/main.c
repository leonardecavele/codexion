/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:22:50 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/13 18:09:29 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "args.h"
#include "objects.h"

int	main(int ac, char **av)
{
	t_errcode	errcode;
	t_args		args;
	t_objects	objects;

	errcode = parse_args(ac - 1, av + 1, &args);
	if (errcode != NO_ERROR)
		return (errcode);
	if (args.noc == 0)
	{
		fprintf(stderr, "invalid number of coders: (can't process 0 coder)");
		return (INVALID_ARG_ERROR);
	}
	errcode = allocate_objects(args, &objects);
	if (errcode == NO_ERROR)
	{
		set_up_objects(args, &objects);
		// run proram and stuff
	}
	destroy_objects(&objects);
	return (errcode);
}
