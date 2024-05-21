# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/18 17:44:12 by marvin            #+#    #+#              #
#    Updated: 2024/05/21 18:30:29 by tibarbos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf
SRC= ./parser/p_1.c ./parser/p_1a.c \
	./parser/p_2.c ./parser/p_2a.c \
	./parser/p_3.c \
	./parser/p_4.c ./parser/p_4a.c ./parser/p_4b.c \
	./parser/p_5.c ./parser/p_5a.c \
	./parser/p_6.c ./parser/p_6a.c \
	./executor/e_main.c ./executor/e_loop.c ./executor/e_action.c \
	./executor/e_close.c ./executor/e_outf.c ./executor/e_pipe.c \
	./executor/e_envs.c ./executor/e_redirs.c \
	./builtin/blt_central.c ./builtin/cd.c ./builtin/echo.c ./builtin/env.c \
	./builtin/exit.c ./builtin/export.c ./builtin/pwd.c ./builtin/unset.c \
	main.c free.c signals.c
OBJ= ${SRC:.c=.o}
LIBS= -lreadline -L./mylib -lft

$(NAME): $(OBJ)
	@cd ./mylib && make
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

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