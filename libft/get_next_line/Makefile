# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/09 11:14:40 by ljeribha          #+#    #+#              #
#    Updated: 2025/03/31 09:20:12 by ljeribha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Werror -Wall -Wextra

INCS = ${LIBFT_PATH}
NAME = get_next_line.a
LIB = get_next_line.c get_next_line_utils.c

OBJS = ${SRCS:.c=.o}
SRCS = ${LIB}

LIBFT = ${LIBFT_PATH}/libft.a
LIBFT_PATH = ../libft

.c.o: ${INCS}
	${CC} -g ${CFLAGS} -c $< -o $@ -I ${INCS}

${NAME}: ${OBJS}
	ar rcs ${NAME} ${OBJS}

${LIBFT}:
	make -C ${LIBFT_PATH}

clean:
	rm -f $(OBJS)

all: $(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all