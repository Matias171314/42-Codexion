NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

INCLUDES = -I includes

SRC = src/main.c \
	src/parsing/parse_config.c \
	src/parsing/parse_number.c \
	src/parsing/parse_policy.c \
	src/init/init_sim.c \
	src/init/init_scheduler.c \
	src/init/init_dongles.c \
	src/init/init_coders.c \
	src/cleanup/destroy_sim.c \
	src/time/time.c \
	src/time/sleep.c \
	src/time/sim_sleep.c \
	src/log/log.c \
	src/state/sim_state.c \
	src/state/coder_state.c \
	src/dongle/dongle.c \
	src/dongle/dongle_pair.c \
	src/heap/heap_compare.c \
	src/heap/heap_push.c \
	src/heap/heap_pop.c \
	src/heap/heap_remove.c \
	src/scheduler/scheduler_request.c \
	src/scheduler/scheduler_grant.c \
	src/scheduler/scheduler_wait.c \
	src/threads/coder_routine.c \
	src/threads/coder_cycle.c \
	src/threads/thread_control.c \
	src/threads/thread_join.c \
	src/monitor/monitor.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c includes/codexion.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re