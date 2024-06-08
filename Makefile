
CC=gcc
CFLAGS=-Wall -g -Iinclude

LDFLAGS=-lreadline


TARGET=minios


SRCS=kernel/kernel.c kernel/system.c kernel/list.c kernel/ls.c kernel/pwd.c kernel/cat.c kernel/mv.c kernel/find.c kernel/cp.c kernel/create_directory.c kernel/tcp_client.c
OBJS=$(SRCS:.c=.o) 


INCLUDE_DIR=include

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)
