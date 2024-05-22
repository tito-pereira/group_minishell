# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/18 17:44:12 by marvin            #+#    #+#              #
#    Updated: 2024/05/22 22:48:24 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name && Utilities
NAME= minishell
RM= rm -rf

# Compiler && Flags
CC= cc
CFLAGS= -Wall -Wextra -Werror

# Source && Object Files
SRC_P= ./src/parser/p_1.c ./src/parser/p_1a.c \
	./src/parser/p_2.c ./src/parser/p_2a.c ./src/parser/p_2a.c \
	./src/parser/p_3.c \
	./src/parser/p_4.c ./src/parser/p_4a.c ./src/parser/p_4b.c \
	./src/parser/p_5.c ./src/parser/p_5a.c \
	./src/parser/p_6.c ./src/parser/p_6a.c
OBJ_P= $({SRC_P:.c=.o})

SRC_E= ./src/executor/e_main.c ./src/executor/e_loop.c \
	./src/executor/e_action.c ./src/executor/e_close.c \
	./src/executor/e_outf.c ./src/executor/e_pipe.c \
	./src/executor/e_envs.c ./src/executor/e_redirs.c
OBJ_E= $({SRC_E:.c=.o})

SRC_B= ./src/builtin/blt_central.c ./src/builtin/cd.c \
	./src/builtin/echo.c ./src/builtin/env.c \
	./src/builtin/exit.c ./src/builtin/export.c \
	./src/builtin/pwd.c ./src/builtin/unset.c
OBJ_B= $({SRC_B:.c=.o})

SRC_M= ./src/main.c ./src/free.c ./src/signals.c
OBJ_M= $({SRC_M:.c=.o})

OBJ= $(OBJ_P) $(OBJ_E) $(OBJ_B) $(OBJ_M)

# Include Libraries
LIB_FT= -L./mylib -lft
LIB_READ= -lreadline
LIBS= $(LIB_FT) $(LIB_READ)

# Rules
$(NAME): $(OBJ)
	@cd ./mylib && make
	@$(CC) -g $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

all: $(NAME)
	@cd ./mylib && make all

clean:
	@cd ./mylib && make clean
	@$(RM) $(OBJ)

fclean:	clean
	@cd ./mylib && make fclean
	@$(RM) $(NAME)

re: fclean all
	@cd ./mylib && make re

test: re
	@make clean
	@clear
	@./$(NAME)