# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/02 by benpicar    	           #+#    #+#              #
#    Updated: 2025/12/02 by benpicar         	      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test_libft
BONUS_NAME = test_libft_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT_DIR = ..
LIBFT = $(LIBFT_DIR)/libft.a

OBJS_DIR = objects/
SRC = test_libft.c
OBJ = $(addprefix $(OBJS_DIR), $(SRC:.c=.o))
BONUS_SRC = test_libft_bonus.c
BONUS_OBJ = $(addprefix $(OBJS_DIR), $(BONUS_SRC:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@echo "🔨 Compilation du tester..."
	@$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "✅ Tester compilé: ./$(NAME)"

$(BONUS_NAME): bonus $(BONUS_OBJ)
	@echo "🔨 Compilation du tester bonus..."
	@$(CC) $(CFLAGS) $(BONUS_OBJ) -L$(LIBFT_DIR) -lft -o $(BONUS_NAME)
	@echo "✅ Tester bonus compilé: ./$(BONUS_NAME)"

$(OBJS_DIR)%.o: %.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

bonus:
	@echo "📚 Compilation de libft bonus..."
	@make bonus -sC $(LIBFT_DIR)

$(LIBFT):
	@echo "📚 Compilation de libft..."
	@make -sC $(LIBFT_DIR)

a: t b

t: $(NAME)
	@echo ""
	@echo "🧪 Lancement des tests..."
	@echo ""
	@./$(NAME)

b: $(BONUS_NAME)
	@echo ""
	@echo "🧪 Lancement des tests bonus..."
	@echo ""
	@./$(BONUS_NAME)

clean:
	@rm -f $(NAME) $(BONUS_NAME)
	@rm -rf $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)
	@echo "🧹 Fichiers de test nettoyés"

fclean: clean
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all bonus t b clean fclean re a
