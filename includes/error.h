/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 13:23:36 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/18 17:24:54 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef int	t_errcode;

typedef enum e_errcodes
{
	NO_ERROR,
	MALLOC_ERROR,
	ARG_COUNT_ERROR,
	INVALID_ARG_ERROR,
	ZERO_CODERS_ERROR,
	THREAD_CREATE_ERROR,
}	t_errcodes;

t_errcode	error_message(t_errcode errcode);

#endif
