#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define TETROMINO_SIZE 4

typedef enum {
  STATE_INIT,
  STATE_PLAYING,
  STATE_PAUSED,
  STATE_GAME_OVER,
  STATE_QUIT
} GameState;

typedef enum {
  INPUT_NONE,
  INPUT_START,
  INPUT_PAUSE,
  INPUT_QUIT,
  INPUT_LEFT,
  INPUT_RIGHT,
  INPUT_DOWN,
  INPUT_DROP,
  INPUT_ROTATE
} InputCommand;

typedef struct {
  int x, y; // позиция фигуры
  int shape[TETROMINO_SIZE][TETROMINO_SIZE];
} Tetromino;

typedef struct {
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  Tetromino current;
  int score;
  int high_score;
  int level;
  GameState state;
} TetrisGame;

void tetris_init(TetrisGame *game);
void tetris_input(TetrisGame *game, InputCommand cmd);
void tetris_update(TetrisGame *game);

bool check_collision(TetrisGame *game, Tetromino *tet);
void place_tetromino(TetrisGame *game);
int clear_lines(TetrisGame *game);
void update_level(TetrisGame *game);
Tetromino generate_random_tetromino(void);

#endif // TETRIS_H
