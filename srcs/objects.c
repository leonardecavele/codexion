/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:09:00 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/13 17:22:31 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

extern t_errcode	allocate_objects(t_args args, t_objects *objects)
{
	(void)args;
	(void)objects;
	return NO_ERROR;
}

extern void	destroy_objects(void)
{
	return ;
}
