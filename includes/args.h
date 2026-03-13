/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:12:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/13 17:22:20 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H

# include <stdbool.h>
# include <stdlib.h>
# include <string.h>

# include "error.h"

typedef enum e_schedules
{
	FIFO = false,
	EDF = true
}	t_schedules;

typedef struct s_args
{
	size_t	noc; // number of coders
	size_t	ttb; // time to burnout
	size_t	ttc; // time to compile
	size_t	ttd; // time to debug
	size_t	ttr; // time to refactor
	size_t	nocr; // number of compiles required
	size_t	dc; // dongle cooldown
	bool	scheduler;
}	t_args;

t_errcode	parse_args(int ac, char **av, t_args *args);

#endif
