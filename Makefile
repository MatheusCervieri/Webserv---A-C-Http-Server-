# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvieira- <mvieira-@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/26 08:06:24 by mvieira-          #+#    #+#              #
#    Updated: 2023/01/02 09:50:02 by mvieira-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++
INCLUDES	=	includes
CPPFLAGS	=	-g -Wall -Werror -Wextra -std=c++98 -I $(INCLUDES)
RM			=	rm -rf
SRCS		=	main.cpp \
				basic_server.cpp \
				parser/Parser.cpp \

OBJS		=	$(SRCS:%.cpp=%.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

.cpp.o:
			@$(CC) $(CPPFLAGS) -c $< -o $(<:.cpp=.o)

clean:
			@$(RM) $(OBJS)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		fclean, all, re