CC:=clang
CFLAGS:=-Wall -Wextra -std=c11 -pedantic
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: all clean check

all: filescan

filescan: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS)
	rm -f filescan

check:
	scan-build -analyze-headers -maxloop 20 $(MAKE)
