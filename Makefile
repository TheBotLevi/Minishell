CC = cc
CFLAGS = -Werror -Wall -Wextra

INCLUDES = inc/minishell.h

SRC_DIR = src
OBJ_DIR = obj

PATH_LIBFT = libft

LIBFT = ${PATH_LIBFT}/libft.a

OBJS = ${SRCS:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}
SRCS = src/mini_main.c \
       src/mini_utils.c \
       src/executions/mini_pipes.c \
       src/executions/mini_pipes_exec.c \
       src/mini_handle_built_in.c \
       src/executions/mini_execute.c \
       src/mini_environment.c \
       src/mini_remove_env.c \
       src/mini_update_env.c \
       src/mini_free.c \
       src/signals/mini_handling_signals.c \
       src/signals/mini_handling_heredoc_signals.c \
       src/mini_init.c \
       src/built_ins/mini_cd.c \
       src/built_ins/mini_echo.c \
       src/built_ins/mini_env.c \
       src/built_ins/mini_exit.c \
       src/built_ins/mini_export.c \
       src/built_ins/mini_pwd.c \
       src/built_ins/mini_unset.c \
       src/redirections/mini_redirections.c \
       src/redirections/mini_handle_heredoc.c \
       src/parsing/mini_tokenize.c \
       src/parsing/mini_quotes.c \
       src/parsing/mini_split.c

MINISHELL = minishell

OBJ_DIRS = ${OBJ_DIR} ${OBJ_DIR}/built_ins ${OBJ_DIR}/executions ${OBJ_DIR}/redirections ${OBJ_DIR}/signals ${OBJ_DIR}/parsing

all: ${OBJ_DIRS} ${LIBFT} ${MINISHELL}

${OBJ_DIRS}:
	mkdir -p ${OBJ_DIRS}

${MINISHELL}: ${OBJS} ${LIBFT}
	${CC} -g ${CFLAGS} -o $@ $^ -lreadline -lhistory

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c ${INCLUDES} | ${OBJ_DIRS}
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
