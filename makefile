$(shell mkdir -p test/bin include)

CC := clang
CFLAGS := -Wall -Wextra -Werror -Wpedantic -std=c17 -Wno-unused-parameter
LDFLAGS := -lpthread -Iinclude -Itest/include

.PHONY: test build clean

build:
	$(CC) $(CFLAGS) $(LDFLAGS) test/run.c -Itest/cases -o test/bin/test 

test: build
	./test/bin/test

clean:
	rm -rf test/bin
