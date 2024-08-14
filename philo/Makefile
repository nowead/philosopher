# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/18 19:05:46 by damin             #+#    #+#              #
#    Updated: 2024/08/14 19:35:45 by damin            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			 =	cc
CFLAGS       =	-Wall -Wextra -Werror #-g -fsanitize=thread

NAME	 =	philo

SRCS		 = main.c philo.c philo_utils.c simulation.c death_checker.c
			
OBJS 	 	 = $(SRCS:.c=.o)

OBJECTS = $(OBJS) $(SUBLIB)
HEADERS = philo.h
DEPENDENCY = $(HEADERS)

all: $(NAME)

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $(NAME)
	@echo "\033[0;32mphilo\033[0m compiled."

%.o: %.c $(DEPENDENCY)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@echo "remove object files"
	
fclean: clean
	@$(RM) $(NAME)
	@echo "remove \033[0;31mphilo.\033[0m"

re: 
	make fclean
	make all

.PHONY: all clean bonus fclean re 
