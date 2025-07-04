
CFLAGS = -Werror -Wextra -std=c99

example: example.c
	$(CC) $(CFLAGS) $^ -o $@
