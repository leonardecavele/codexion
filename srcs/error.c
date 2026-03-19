/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:01:36 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/19 12:13:38 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "error.h"

static void	usage(void)
{
	fprintf(
		stderr,
		"Usage:\n"
		"  ./codexion number_of_coders time_to_burnout time_to_compile "
		"time_to_debug time_to_refactor number_of_compiles_required "
		"dongle_cooldown scheduler [debug|DEBUG]\n"
		"\n"
		"Arguments:\n"
		"  number_of_coders            number of coders\n"
		"  time_to_burnout             time before burnout (ms)\n"
		"  time_to_compile             time compiling (ms)\n"
		"  time_to_debug               time debugging (ms)\n"
		"  time_to_refactor            time refactoring (ms)\n"
		"  number_of_compiles_required number of compiles required\n"
		"  dongle_cooldown             dongle cooldown (ms)\n"
		"  scheduler                   fifo|edf\n"
		"  [debug]                     enable extra debug logs\n"
		);
}

extern t_errcode	error_message(t_errcode errcode)
{
	if (errcode == NO_ERROR)
		;
	else if (errcode == MALLOC_ERROR)
		fprintf(stderr, "cannot allocate memory, malloc failed\n");
	else if (errcode == ARG_COUNT_ERROR)
		usage();
	else if (errcode == INVALID_ARG_ERROR)
		usage();
	else if (errcode == ZERO_CODERS_ERROR)
		fprintf(stderr, "invalid number of coders: (can't process 0 coder)\n");
	else if (errcode == THREAD_CREATE_ERROR)
		fprintf(stderr, "cannot create thread\n");
	return (errcode);
}
