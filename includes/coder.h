/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:12:56 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 15:38:17 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H
# define CODER_H

# include <pthread.h>

# include "args.h"
# include "objects.h"
# include "error.h"

t_errcode	start_session(t_args *args, t_objects *objects);
void		wait_session(t_objects *objects, size_t n_threads);

#endif
