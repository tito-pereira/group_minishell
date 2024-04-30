# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/18 17:44:12 by marvin            #+#    #+#              #
#    Updated: 2024/04/17 13:47:30 by tibarbos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf
SRC= main.c free.c signals.c\
	p_1.c p_2.c p_2a.c p_3.c p_3a.c p_4.c p_4a.c p_5.c p_5a.c
BLT= builtins/cd.c builtins/echo.c builtins/env.c builtins/exit.c \
	builtins/export.c builtins/pwd.c builtins/unset.c
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