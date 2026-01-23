# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/03 13:00:00 by benpicar          #+#    #+#              #
#    Updated: 2026/01/23 16:55:52 by benpicar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nom de l'exécutable
NAME = tester_gnl
RUN_ALL = run_all
GNL_DIR = ../

# Compilateur et flags
CC = cc
INCLUDES = -I $(GNL_DIR)
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
BUFFER_SIZE ?= 42

# Fichiers sources
TESTER_SRC = tester_gnl.c
GNL_SRC = $(GNL_DIR)get_next_line.c $(GNL_DIR)get_next_line_utils.c

# Fichiers objets
OBJS_DIR = objects/
TESTER_OBJ = $(addprefix $(OBJS_DIR), $(TESTER_SRC:.c=.o))
GNL_OBJ = $(addprefix $(OBJS_DIR), $(notdir $(GNL_SRC:.c=.o)))
OBJ = $(TESTER_OBJ) $(GNL_OBJ)

# Headers
HEADER = $(GNL_DIR)get_next_line.h

# Couleurs
GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

# Règles
all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) $(OBJ) -o $(NAME)
	@echo "$(GREEN)✓ Tester GNL compilé avec succès !$(NC)"
	@echo "$(GREEN)  BUFFER_SIZE=$(BUFFER_SIZE)$(NC)"

$(OBJS_DIR)%.o: %.c $(HEADER) | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) -c $< -o $@

$(OBJS_DIR)%.o: $(GNL_DIR)%.c $(HEADER) | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# Compiler le programme run_all
$(RUN_ALL): run_all.c
	@$(CC) -Wall -Wextra -Werror run_all.c -o $(RUN_ALL)
	@echo "$(GREEN)✓ Programme run_all compilé$(NC)"

# Compiler et lancer tous les tests
test: $(RUN_ALL)
	@./$(RUN_ALL)

# Tester avec différentes valeurs de BUFFER_SIZE
test1:
	@echo "$(GREEN)=== Test avec BUFFER_SIZE=1 ===$(NC)"
	@$(MAKE) fclean > /dev/null
	@$(MAKE) BUFFER_SIZE=1 > /dev/null
	-@./$(NAME)

test10:
	@echo "$(GREEN)=== Test avec BUFFER_SIZE=10 ===$(NC)"
	@$(MAKE) fclean > /dev/null
	@$(MAKE) BUFFER_SIZE=10 > /dev/null
	-@./$(NAME)

test42:
	@echo "$(GREEN)=== Test avec BUFFER_SIZE=42 ===$(NC)"
	@$(MAKE) fclean > /dev/null
	@$(MAKE) BUFFER_SIZE=42 > /dev/null
	-@./$(NAME)

test1000:
	@echo "$(GREEN)=== Test avec BUFFER_SIZE=1000 ===$(NC)"
	@$(MAKE) fclean > /dev/null
	@$(MAKE) BUFFER_SIZE=1000 > /dev/null
	-@./$(NAME)

test10000:
	@echo "$(GREEN)=== Test avec BUFFER_SIZE=10000 ===$(NC)"
	@$(MAKE) fclean > /dev/null
	@$(MAKE) BUFFER_SIZE=10000 > /dev/null
	-@./$(NAME)

# Tester tous les BUFFER_SIZE
testall: test1 test10 test42 test1000 test10000
	@echo "$(GREEN)✓ Tous les tests BUFFER_SIZE terminés !$(NC)"

# Nettoyage
clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(RED)✗ Fichiers objets supprimés$(NC)"

fclean: clean
	@rm -f $(NAME) $(RUN_ALL)
	@echo "$(RED)✗ Exécutables supprimés$(NC)"

re: fclean all

# Cibles phony
.PHONY: all test test1 test10 test42 test1000 test10000 testall clean fclean re
