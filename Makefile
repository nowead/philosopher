# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: damin <damin@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/18 19:05:46 by damin             #+#    #+#              #
#    Updated: 2024/07/22 16:27:23 by damin            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			 =	cc
CFLAGS		 =	-Wall -Wextra -Werror

ifdef WITH_BONUS
	NAME		 =	philo_bonus
else
	NAME		 =	philo
endif

SRCS		 = philo.c
BONUS_SRCS	 = 
			
OBJS 	 	 = $(SRCS:.c=.o)
BONUS_OBJS   = $(BONUS_SRCS:.c=.o)

SUBDIR		 = ./libft/
SUBLIB		 = $(SUBDIR)libft.a

ifdef WITH_BONUS
	OBJECTS = $(BONUS_OBJS) $(SUBLIB)
	HEADERS = philo.h philo_bonus.h
	DEPENDENCY = $(HEADERS)
else
	OBJECTS = $(OBJS) $(SUBLIB)
	HEADERS = philo.h
	DEPENDENCY = $(HEADERS)
endif

all: $(NAME)

bonus:
ifeq (1, $(shell test -e philo.o && echo 1))
	@$(RM) philo.o philo
endif
	make WITH_BONUS=1 all

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $(NAME)
	@echo "\033[0;32mphilo\033[0m compiled."

%.o: %.c $(DEPENDENCY)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS) $(BONUS_OBJS)
	@echo "remove object files"
endif
	
fclean: clean
	@$(RM) $(NAME)
	@echo "remove \033[0;31mphilo.\033[0m"
endif

re: 
	make fclean
	make all

.PHONY: all clean bonus fclean re 
