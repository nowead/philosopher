# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: damin <damin@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/14 19:37:55 by damin             #+#    #+#              #
#    Updated: 2024/08/19 22:14:40 by damin            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			 =	cc
CFLAGS       =	-Wall -Wextra -Werror
#-fsanitize=thread -g

NAME	 =	philo_bonus

SRCS		 = main_bonus.c philo_bonus.c philo_utils_bonus.c simulation_bonus.c \
				death_checker_bonus.c
			
OBJS 	 	 = $(SRCS:.c=.o)

OBJECTS = $(OBJS) $(SUBLIB)
HEADERS = philo.h
DEPENDENCY = $(HEADERS)

all: $(NAME)

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $(NAME)
	@echo "\033[0;32mphilo_bonus\033[0m compiled."

%.o: %.c $(DEPENDENCY)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@echo "remove object files"
	
fclean: clean
	@$(RM) $(NAME)
	@echo "remove \033[0;31mphilo_bonus.\033[0m"

re: 
	make fclean
	make all

.PHONY: all clean fclean re 
