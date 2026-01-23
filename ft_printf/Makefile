# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/23 10:00:00 by benpicar          #+#    #+#              #
#    Updated: 2026/01/23 17:13:16 by benpicar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= tester_printf
NAME_BONUS	= tester_printf_bonus
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3
RM			= rm -f

# Couleurs
GREEN		= \033[0;32m
RED			= \033[0;31m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
NC			= \033[0m

# Fichiers
SRCS		= tester_printf.c
SRCS_BONUS	= tester_printf_bonus.c
OBJS_DIR	= objects/
OBJS		= $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))
OBJS_BONUS	= $(addprefix $(OBJS_DIR), $(SRCS_BONUS:.c=.o))

# Chemins pour ft_printf (à adapter selon la structure de votre projet)
PRINTF_DIR	= ..
PRINTF_LIB	= $(PRINTF_DIR)/libftprintf.a
PRINTF_INC	= -I$(PRINTF_DIR)/includes

# Règle principale
all: $(NAME)

# Compilation du tester
$(NAME): $(OBJS) $(PRINTF_LIB)
	@echo "$(BLUE)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(PRINTF_LIB) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(NC)"

# Compilation du tester bonus
bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS) $(PRINTF_LIB)
	@echo "$(BLUE)Linking $(NAME_BONUS)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(PRINTF_LIB) -o $(NAME_BONUS)
	@echo "$(GREEN)✓ $(NAME_BONUS) compiled successfully!$(NC)"

# Compilation de libftprintf.a
$(PRINTF_LIB):
	@echo "$(YELLOW)Compiling libftprintf.a...$(NC)"
	@$(MAKE) -C $(PRINTF_DIR) > /dev/null 2>&1 || \
	(echo "$(RED)Error: Could not compile libftprintf.a$(NC)" && \
	 echo "$(YELLOW)Make sure you have ft_printf implementation in: $(PRINTF_DIR)$(NC)" && \
	 echo "$(YELLOW)Or update PRINTF_DIR in this Makefile$(NC)" && exit 1)
	@echo "$(GREEN)✓ libftprintf.a compiled!$(NC)"

# Compilation des objets
$(OBJS_DIR)%.o: %.c | $(OBJS_DIR)
	@echo "$(BLUE)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(PRINTF_INC) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# Nettoyage
clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@$(RM) -r $(OBJS_DIR)
	@$(MAKE) -C $(PRINTF_DIR) clean > /dev/null 2>&1 || true

fclean: clean
	@echo "$(RED)Cleaning $(NAME) and libraries...$(NC)"
	@$(RM) $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(PRINTF_DIR) fclean > /dev/null 2>&1 || true

re: fclean all

# Compilation et exécution
run: $(NAME)
	@echo "$(GREEN)Running mandatory tests...$(NC)"
	@./$(NAME)

run_bonus: $(NAME_BONUS)
	@echo "$(GREEN)Running bonus tests...$(NC)"
	@./$(NAME_BONUS)

run_all: $(NAME) $(NAME_BONUS)
	@echo "$(GREEN)Running mandatory tests...$(NC)"
	@./$(NAME)
	@echo ""
	@echo "$(GREEN)Running bonus tests...$(NC)"
	@./$(NAME_BONUS)

# Test avec valgrind
valgrind: $(NAME)
	@echo "$(YELLOW)Running mandatory tests with valgrind...$(NC)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

valgrind_bonus: $(NAME_BONUS)
	@echo "$(YELLOW)Running bonus tests with valgrind...$(NC)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME_BONUS)

# Aide
help:
	@echo "$(BLUE)╔════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(BLUE)║              FT_PRINTF TESTER - MAKEFILE HELP              ║$(NC)"
	@echo "$(BLUE)╠════════════════════════════════════════════════════════════╣$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make$(NC) or $(GREEN)make all$(NC)     - Compile mandatory tester         $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make bonus$(NC)           - Compile bonus tester                $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make run$(NC)             - Run mandatory tests                 $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make run_bonus$(NC)       - Run bonus tests                     $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make run_all$(NC)         - Run all tests (mandatory + bonus)   $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make valgrind$(NC)        - Run mandatory tests with valgrind   $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make valgrind_bonus$(NC)  - Run bonus tests with valgrind       $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make clean$(NC)           - Remove object files                $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make fclean$(NC)          - Remove all compiled files          $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make re$(NC)              - Recompile everything                $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make help$(NC)            - Show this help message              $(BLUE)║$(NC)"
	@echo "$(BLUE)╠════════════════════════════════════════════════════════════╣$(NC)"
	@echo "$(BLUE)║$(NC) $(YELLOW)Note:$(NC) Make sure PRINTF_DIR points to your ft_printf    $(BLUE)║$(NC)"
	@echo "$(BLUE)║       implementation (default: parent directory)           ║$(NC)"
	@echo "$(BLUE)╚════════════════════════════════════════════════════════════╝$(NC)"

.PHONY: all bonus clean fclean re run run_bonus run_all valgrind valgrind_bonus════════════════════════════════════╗$(NC)"
	@echo "$(BLUE)║              FT_PRINTF TESTER - MAKEFILE HELP              ║$(NC)"
	@echo "$(BLUE)╠════════════════════════════════════════════════════════════╣$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make$(NC) or $(GREEN)make all$(NC)     - Compile the tester              $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make run$(NC)             - Compile and run tests              $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make valgrind$(NC)        - Run tests with valgrind            $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make clean$(NC)           - Remove object files                $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make fclean$(NC)          - Remove all compiled files          $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make re$(NC)              - Recompile everything                $(BLUE)║$(NC)"
	@echo "$(BLUE)║$(NC) $(GREEN)make help$(NC)            - Show this help message              $(BLUE)║$(NC)"
	@echo "$(BLUE)╠════════════════════════════════════════════════════════════╣$(NC)"
	@echo "$(BLUE)║$(NC) $(YELLOW)Note:$(NC) Make sure PRINTF_DIR points to your ft_printf    $(BLUE)║$(NC)"
	@echo "$(BLUE)║       implementation (default: current directory)          ║$(NC)"
	@echo "$(BLUE)╚════════════════════════════════════════════════════════════╝$(NC)"

.PHONY: all clean fclean re run valgrind help
