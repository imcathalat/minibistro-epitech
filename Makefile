CC     = gcc
CFLAGS = -Wall -Wextra -Werror
SRC    = main.c
OBJ    = $(SRC:.c=.o)
NAME   = calc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
	rm -f $(OBJ) calc


fclean: clean
	rm -f $(NAME)

re: fclean all
