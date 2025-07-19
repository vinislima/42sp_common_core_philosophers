# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vinda-si <vinda-si@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/16 21:28:13 by vinda-si          #+#    #+#              #
#    Updated: 2025/07/18 22:35:50 by vinda-si         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -MMD -MP -pthread
MAKEFLAGS += --no-print-directory

#******************************************************************************#
#                                   PATH                                       #
#******************************************************************************#

SRCDIR = philo/src
OBJDIR = build
INCDIR = philo/includes

#******************************************************************************#
#                                   COLOR                                      #
#******************************************************************************#

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m

#******************************************************************************#
#                                   FILES                                      #
#******************************************************************************#

HEADERS = $(INCDIR)/philo.h

MAIN_SRC = main.c \
		   init.c \
		   monitor.c \
		   simulation.c \
		   utils.c \
		   forks.c

SRC = $(MAIN_SRC)

OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
DEPS = $(addprefix $(OBJDIR)/, $(SRC:.c=.d))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	echo "$(GREEN)Philosophers compiled!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@
	echo "$(BLUE)Compiling: $(notdir $<)$(RESET)"

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
	echo "$(RED)Objects and Deps removed$(RESET)"

fclean: clean
	rm -f $(NAME)
	echo "$(RED)Executable $(NAME) removed$(RESET)"

re: fclean all
	echo "$(CYAN)Project recompiled!$(RESET)"

#******************************************************************************#
#                              MODO DEBUG                                      #
#******************************************************************************#

valgrind: $(NAME)
	echo "$(PURPLE)Running with Valgrind...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

gdb: $(NAME)
	echo "$(PURPLE)Starting GDB...$(RESET)"
	gdb ./$(NAME)

#******************************************************************************#
#                              BUILD TYPES                                     #
#******************************************************************************#

norm:
	if command -v norminette >/dev/null 2>&1; then \
		echo "$(YELLOW)Checking norm...$(RESET)"; \
		norminette $(SRCDIR) $(INCDIR); \
	else \
		echo "$(YELLOW)Norminette not found$(RESET)"; \
	fi

help:
	echo "$(CYAN)Available commands:$(RESET)"
	echo "$(GREEN)  make           $(RESET)- Compile the project"
	echo "$(GREEN)  make valgrind  $(RESET)- Run with valgrind"
	echo "$(GREEN)  make gdb       $(RESET)- Debug with gdb"
	echo "$(GREEN)  make clean     $(RESET)- Remove objects"
	echo "$(GREEN)  make fclean    $(RESET)- Remove all"
	echo "$(GREEN)  make re        $(RESET)- Recompile all"
	echo "$(GREEN)  make norm      $(RESET)- Check norm"

-include $(DEPS)
.PHONY: all clean fclean re valgrind gdb norm help
.SILENT: 
