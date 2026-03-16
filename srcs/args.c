/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:27:31 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 13:35:03 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"

static t_errcode	parse_int_arg(const char *s, size_t *arg)
{
	size_t	i;
	size_t	value;

	i = -1;
	while (s[++i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
		{
			fprintf(stderr, "invalid arg: '%s' (must be only digits)\n", s);
			return (INVALID_ARG_ERROR);
		}
	}
	value = atoi(s);
	if (value < 0)
	{
		fprintf(stderr, "invalid arg: '%s' (must be positive integer)\n", s);
		return (INVALID_ARG_ERROR);
	}
	*arg = value;
	return (NO_ERROR);
}

static t_errcode	parse_scheduler_arg(const char *s, bool *arg)
{
	if (!strcmp(s, "FIFO") || !strcmp(s, "fifo"))
		*arg = FIFO;
	else if (!strcmp(s, "EDF") || !strcmp(s, "edf"))
		*arg = EDF;
	else
		return (INVALID_ARG_ERROR);
	return (NO_ERROR);
}

static t_errcode	parse_debug_arg(const char *s, bool *arg, int ac)
{
	if (ac < 9)
		return (NO_ERROR);
	if (!strcmp(s, "DEBUG") || !strcmp(s, "debug"))
		*arg = true;
	else
		return (INVALID_ARG_ERROR);
	return (NO_ERROR);
}

extern t_errcode	parse_args(int ac, char **av, t_args *args)
{
	t_errcode	errcode;

	if (ac < 8 || ac > 9)
		return (ARG_COUNT_ERROR);
	errcode = parse_int_arg(*av++, &args->noc);
	errcode |= parse_int_arg(*av++, &args->ttb);
	errcode |= parse_int_arg(*av++, &args->ttc);
	errcode |= parse_int_arg(*av++, &args->ttd);
	errcode |= parse_int_arg(*av++, &args->ttr);
	errcode |= parse_int_arg(*av++, &args->nocr);
	errcode |= parse_int_arg(*av++, &args->dc);
	errcode |= parse_scheduler_arg(*av++, &args->scheduler);
	errcode |= parse_debug_arg(*av, &args->debug, ac);
	if (errcode != NO_ERROR)
		return (INVALID_ARG_ERROR);
	return (NO_ERROR);
}
