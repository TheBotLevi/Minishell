# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 15:20:10 by ljeribha          #+#    #+#              #
#    Updated: 2024/11/06 11:56:17 by ljeribha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBC = ar rc
LIBR = ranlib
CC = cc
CFLAGS = -Wall -Werror -Wextra
INCS = .
NAME = libftprintf.a
LIB = ft_printf.c ft_putchar.c ft_putstr.c ft_strlen.c
ADDFONC = prt_hexa.c prt_int.c prt_str.c prt_ptr.c prt_us.c
OBJS = $(SRCS:.c=.o)
SRCS = ${LIB} ${ADDFONC}

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I ${INCS}

${NAME}: ${OBJS}
	${LIBC} ${NAME} ${OBJS}
	${LIBR} ${NAME}

clean:
	rm -f $(OBJS)

all: $(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all