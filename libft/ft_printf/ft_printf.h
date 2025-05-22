/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:54:27 by ljeribha          #+#    #+#             */
/*   Updated: 2024/11/06 15:40:51 by ljeribha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>

void	ft_putstr(char *str);
void	ft_putchar(char c);
int		ft_strlen(const char *str);
int		prt_hexa(unsigned int nb, bool upper_case);
int		prt_ptr(void *pt);
int		prt_us(unsigned int nbr);
int		prt_int(int n);
int		prt_str(const char *str);
int		ft_printf(const char *str, ...);

#endif