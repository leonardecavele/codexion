/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:22:50 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/13 14:46:18 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "error.h"
#include "parsing.h"

int	main(int ac, char **av)
{
	t_errcode	errcode;
	t_args		args;

	errcode = NO_ERROR;
	errcode = parse_args(ac - 1, av + 1, &args);
	if (errcode != NO_ERROR)
		return (errcode);
	printf("%zu\n", args.noc);
	printf("%zu\n", args.ttb);
	printf("%zu\n", args.ttc);
	printf("%zu\n", args.ttd);
	printf("%zu\n", args.ttr);
	printf("%zu\n", args.nocr);
	printf("%zu\n", args.dc);
	printf("%d\n", args.scheduler);
	return (errcode);
}
