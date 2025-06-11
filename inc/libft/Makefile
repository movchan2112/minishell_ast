# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/27 16:17:48 by bmynbyae          #+#    #+#              #
#    Updated: 2025/05/09 17:43:38 by bmynbyae         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = ft_atoi.c \
		ft_bzero.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_itoa.c \
		ft_memset.c \
		ft_putchar_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_putstr.c \
		ft_split.c \
		ft_strchr.c \
		ft_strcmp.c \
		ft_strcpy.c \
		ft_strdup.c \
		ft_strjoin.c \
		ft_strlen.c \
		ft_strndup.c \
		ft_substr.c
OBJS = $(SRCS:.c=.o)
HEADER = libft.h

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
