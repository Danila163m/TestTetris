CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g -I./src/brick_game/tetris
LDFLAGS = -lncurses
CHECKFLAGS = `pkg-config --cflags --libs check`
PREFIX = /usr/local
DESTDIR =

SRC = src/brick_game/tetris/tetris.c src/gui/cli/ncurses.c
OBJ = $(SRC:.c=.o)
TEST_SRC = tests/test_tetris.c
TEST_OBJ = $(TEST_SRC:.c=.o)

.PHONY: all clean install uninstall test gcov_report dist

all: brickgame

brickgame: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: brickgame
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 brickgame $(DESTDIR)$(PREFIX)/bin/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/brickgame

clean:
	rm -f $(OBJ) $(TEST_OBJ) brickgame *.gcda *.gcno *.gcov

test: CFLAGS += $(CHECKFLAGS)
test: LDFLAGS += $(CHECKFLAGS)
test: tetris_test

tetris_test: $(TEST_SRC) $(filter-out src/gui/cli/ncurses.c, $(OBJ))
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	./tetris_test

gcov_report: CFLAGS += -fprofile-arcs -ftest-coverage
gcov_report: LDFLAGS += -fprofile-arcs -ftest-coverage
gcov_report: clean brickgame test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	echo "Coverage report generated in coverage_report/index.html"

dist:
	tar czvf brickgame.tar.gz src Makefile README.md
