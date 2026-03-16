/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldecavel <ldecavel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:18:32 by ldecavel          #+#    #+#             */
/*   Updated: 2026/03/16 18:26:17 by ldecavel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_H
# define HELPERS_H

# include <sys/time.h>
# include <stddef.h>

size_t	current_time_ms(void);
size_t	elapsed_time_ms(size_t start_ms);

#endif
