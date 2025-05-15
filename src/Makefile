# Компилятор и флаги
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g -I./src/brick_game/tetris
LDFLAGS = -lncurses
CHECKFLAGS = `pkg-config --cflags --libs check`

# Пути к исходникам
SRC_TETRIS = src/brick_game/tetris/tetris.c
SRC_GUI = src/gui/cli/ncurses.c
SRC = $(SRC_TETRIS) $(SRC_GUI)

OBJ = $(SRC:.c=.o)

TEST_SRC = tests/test_tetris.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# Имя исполняемого файла
TARGET = brickgame
TEST_TARGET = tetris_test

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Компиляция отдельных объектов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Запуск тестов с check
test: CFLAGS += $(CHECKFLAGS)
test: LDFLAGS += $(CHECKFLAGS)
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC) $(SRC_TETRIS:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET) *.gcda *.gcno *.gcov


