# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/30 13:17:41 by tibarbos          #+#    #+#              #
#    Updated: 2024/05/11 11:43:19 by tibarbos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= builtft
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf
SRC= cd.c echo.c env.c exit.c export.c blt_main.c pwd.c unset.c \
	../free.c
OBJ= ${SRC:.c=.o}
LIBS= -lreadline -L../mylib -lft

$(NAME): $(OBJ)
	cd ../mylib && make
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

all: $(NAME)

clean:
	@$(RM) $(OBJ)

fclean:	clean
	@$(RM) $(NAME)

re: fclean all