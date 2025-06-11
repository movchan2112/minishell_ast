# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/09 17:50:41 by bmynbyae          #+#    #+#              #
#    Updated: 2025/05/20 12:00:00 by bmynbyae         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Executable ---
NAME = minishell

# --- Compiler and Flags ---
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_PATH) -g
LDFLAGS = -lreadline
RM = rm -f

# --- Directories ---
SRC_DIR = src/
PARSING_DIR = $(SRC_DIR)parsing/
EXE_DIR = $(SRC_DIR)execution/
OBJ_DIR = obj/
INC_DIR = inc/
LIBFT_PATH = inc/libft/

# --- Source Files --- redo - exec_tree
SRCS = \
	$(SRC_DIR)main.c \
	$(EXE_DIR)exec_tree.c \
	$(PARSING_DIR)build_in.c \
	$(PARSING_DIR)helpers.c \
	$(PARSING_DIR)buildin_extra1.c \
	$(PARSING_DIR)buildin_extra2.c \
	$(PARSING_DIR)buildin_extra3.c \
	$(PARSING_DIR)buildin_extra4.c \
	$(PARSING_DIR)buildin_extra5.c \
	$(PARSING_DIR)cmd_cheker.c \
	$(PARSING_DIR)expand_vars.c \
	$(PARSING_DIR)expand_vars1.c \
	$(PARSING_DIR)free.c \
	$(PARSING_DIR)handle_operators.c \
	$(PARSING_DIR)line.c \
	$(PARSING_DIR)line_validation.c \
	$(PARSING_DIR)my_strtok.c \
	$(PARSING_DIR)token_utils_1.c \
	$(PARSING_DIR)token_utils_2.c \
	$(PARSING_DIR)token_utils_3.c \
	$(PARSING_DIR)cleaner.c \
	$(PARSING_DIR)env.c \
	$(PARSING_DIR)env_helper.c \
	$(PARSING_DIR)external.c \
	$(PARSING_DIR)ft.c \
	$(PARSING_DIR)ast1.c \
	$(PARSING_DIR)ast2.c \
	$(PARSING_DIR)ast3.c \
	$(PARSING_DIR)ast4.c \
	$(PARSING_DIR)ast5.c \
	$(PARSING_DIR)print.c \
	$(PARSING_DIR)token_utils.c

# --- Object Files ---
OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# --- Libft Archive ---
LIBFT_ARCHIVE = $(LIBFT_PATH)libft.a

# --- Default Rule ---
all: $(NAME)

# --- Build Executable ---
$(NAME): $(OBJS) $(LIBFT_ARCHIVE)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT_ARCHIVE) $(LDFLAGS)
	@echo "$(NAME) created successfully."

# --- Build Libft ---
$(LIBFT_ARCHIVE):
	@echo "Making Libft..."
	@$(MAKE) -s -C $(LIBFT_PATH)

# --- Compile Rule ---
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# --- Cleaning ---
clean:
	@echo "Cleaning object files..."
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -s -C $(LIBFT_PATH) clean

fclean: clean
	@echo "Cleaning $(NAME)..."
	@$(RM) $(NAME)
	@$(MAKE) -s -C $(LIBFT_PATH) fclean

re: fclean all

# --- Phony ---
.PHONY: all clean fclean re
