CC = cc
CFLAGS = -Werror -Wall -Wextra

INCLUDES = inc/minishell.h

SRC_DIR = src
OBJ_DIR = obj

PATH_LIBFT = libft

LIBFT = ${PATH_LIBFT}/libft.a

SRCS = src/main.c src/utils1.c
OBJS = ${SRCS:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

MINISHELL = minishell

all: ${OBJ_DIR} ${LIBFT} ${MINISHELL}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

${MINISHELL}: ${OBJS} ${LIBFT}
	${CC} -g ${CFLAGS} -o $@ $^ -lreadline -lhistory

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c ${INCLUDES}
	${CC} -g ${CFLAGS} -c $< -o $@

${LIBFT}:
	${MAKE} -C ${PATH_LIBFT}

clean:
	rm -f ${OBJS}
	rm -rf ${OBJ_DIR}
	${MAKE} -C ${PATH_LIBFT} clean

fclean: clean
	rm -f ${MINISHELL}
	${MAKE} -C ${PATH_LIBFT} fclean

re: fclean all

.PHONY: all clean fclean re