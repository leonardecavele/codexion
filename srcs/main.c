/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:22:50 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/13 17:19:50 by ldecavel         ###   ########.fr       */
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

	printf("%zu\n", args.noc);
	printf("%zu\n", args.ttb);
	printf("%zu\n", args.ttc);
	printf("%zu\n", args.ttd);
	printf("%zu\n", args.ttr);
	printf("%zu\n", args.nocr);
	printf("%zu\n", args.dc);
	printf("%d\n", args.scheduler);
	errcode = allocate_objects(args, &objects);
	if (errcode != NO_ERROR)
		return (errcode);

	return (errcode);
}
