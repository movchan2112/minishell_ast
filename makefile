CC = gcc
CFLAGS = 
LDFLAGS = -lreadline -lncurses

SRCS = main.c my_strtok.c env.c cmd_cheker.c tokens.c cleaner.c build_in.c
OBJS = $(SRCS:.c=.o)
NAME = minishell

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
