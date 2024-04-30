# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tibarbos <tibarbos@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/30 13:17:41 by tibarbos          #+#    #+#              #
#    Updated: 2024/04/30 14:28:51 by tibarbos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= builtft
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf
SRC= cd.c echo.c env.c exit.c export.c pwd.c unset.c
OBJ= ${SRC:.c=.o}
LIBS= -L../mylib -lft

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

all: $(NAME)

clean:
	@$(RM) $(OBJ)

fclean:	clean
	@$(RM) $(NAME)

re: fclean all