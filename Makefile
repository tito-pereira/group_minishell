# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/18 17:44:12 by marvin            #+#    #+#              #
#    Updated: 2024/05/08 13:53:12 by tibarbos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf
SRC= main.c free.c signals.c\
	p_1.c p_1a.c p_2.c p_2a.c p_3.c p_3a.c p_4.c p_4a.c p_5.c p_6.c p_6a.c\
	e_1.c
OBJ= ${SRC:.c=.o}
LIBS= -lreadline -L./mylib -lft

$(NAME): $(OBJ)
	@cd ./mylib && make
#	@cd ./builtin && make
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

all: $(NAME)
	@cd ./mylib && make all
#	@cd ./builtin && make all

clean:
	@cd ./mylib && make clean
#	@cd ./builtin && make clean
	@$(RM) $(OBJ)

fclean:	clean
	@cd ./mylib && make fclean
#	@cd ./builtin && make fclean
	@$(RM) $(NAME)

re: fclean all
	@cd ./mylib && make re
#	@cd ./builtin && make re

test: re
	@make clean
	@clear
	@./$(NAME)