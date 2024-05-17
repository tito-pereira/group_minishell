# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/30 13:17:41 by tibarbos          #+#    #+#              #
#    Updated: 2024/05/17 15:40:12 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= builtft
CC= cc
CFLAGS= -Wall -Wextra -Werror
RM= rm -rf
SRC= blt_central.c cd.c echo.c env.c exit.c export.c pwd.c unset.c
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