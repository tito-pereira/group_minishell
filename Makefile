# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/18 17:44:12 by marvin            #+#    #+#              #
#    Updated: 2024/05/09 12:49:55 by tibarbos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf
SRC= ./lexer/p_1.c ./lexer/p_1a.c \
	./parser/p_2.c ./parser/p_2a.c ./parser/p_3.c ./parser/p_3a.c \
	./parser/p_4.c ./parser/p_4a.c ./parser/p_5.c ./parser/p_6.c \
	./parser/p_6a.c \
	./executor/e_1.c \
	main.c free.c signals.c
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