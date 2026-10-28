FILES = src/main.c src/coder.c src/destroy.c src/dongles.c src/dongles_extra.c src/cleanup.c src/priority_queue.c src/heap_extra.c src/simulation_utils.c src/init_utils.c src/init.c src/monitor.c src/parsing.c src/simulation.c src/time_utils.c src/utils.c src/init_sync.c

HEADER = src/codexion.h

NAME = codexion

OBJS = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -Wall -Wextra -Werror -pthread $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) -Wall -Wextra -Werror -pthread -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re