# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ljeribha <ljeribha@student.42luxembourg    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/19 14:43:37 by ljeribha          #+#    #+#              #
#    Updated: 2025/03/31 08:44:47 by ljeribha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra

INCS = .
NAME = libft.a
LIBC = ar rcs

LIB = ft_strlen.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c
ADDFONC = ft_substr.c ft_strjoin.c ft_split.c ft_strtrim.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putnbr_fd.c ft_putendl_fd.c
BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c ft_lstsize.c ft_lstlast.c

GET_NEXT_LINE_PATH = get_next_line
FT_PRINTF_PATH = ft_printf

GET_NEXT_LINE = ${GET_NEXT_LINE_PATH}/get_next_line.a
FT_PRINTF = ${ft_PRINTF_PATH}/libftprintf.a

BONUS_OBJS = ${BONUS:.c=.o}
OBJS = ${SRCS:.c=.o}
SRCS = ${LIB} ${ADDFONC}

all: ${NAME}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@ -I ${INCS}

${NAME}: ${OBJS} ${FT_PRINTF} ${GET_NEXT_LINE}
	${LIBC} ${NAME} ${OBJS}

${FT_PRINTF}:
	${MAKE} -C ${FT_PRINTF_PATH}

${GET_NEXT_LINE}:
	${MAKE} -C ${GET_NEXT_LINE_PATH}

clean:
	${MAKE} -C ${FT_PRINTF_PATH} clean
	${MAKE} -C ${GET_NEXT_LINE_PATH} clean
	rm -f ${OBJS} ${BONUS_OBJS}

fclean: clean
	${MAKE} -C ${FT_PRINTF_PATH} fclean
	${MAKE} -C ${GET_NEXT_LINE_PATH} fclean
	rm -f ${NAME}

re: fclean all

bonus : ${OBJS} ${BONUS_OBJS}
	${LIBC} ${NAME} ${OBJS} ${BONUS_OBJS}
