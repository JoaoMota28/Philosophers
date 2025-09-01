NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=thread
INCLUDE	= -I inc/

SRCS = main.c manager.c init.c actions.c utils.c validation.c

OBJS_PATH = obj/
OBJS = $(SRCS:%.c=$(OBJS_PATH)%.o)

all: $(NAME)

$(OBJS_PATH)%.o: %.c | $(OBJS_PATH)
	@$(CC) $(CFLAGS) $(INCLUDE) -pthread -c $(<) -o $(@) 

$(OBJS_PATH):
	@mkdir -p $@

$(NAME): $(OBJS) $(OBJS_PATH)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(OBJS)
	@rm -rf obj

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re