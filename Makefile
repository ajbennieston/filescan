CC:=clang
CFLAGS:=-Wall -Wextra -std=c99 -pedantic
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: all clean

all: filescan

filescan: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS)
	rm -f filescan
