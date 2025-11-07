CC = gcc
CFLAGS = -g -Wall -I./include
SRCS = main.c src/song.c
OBJS = $(SRCS:.c=.o)
TARGET = music_player

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
