#include "tetris.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const int tetromino_shapes[7][TETROMINO_SIZE][TETROMINO_SIZE] = {
  // I
  {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
  },
  // O
  {
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
  },
  // T
  {
    {0,0,0,0},
    {1,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
  },
  // S
  {
    {0,0,0,0},
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0}
  },
  // Z
  {
    {0,0,0,0},
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
  },
  // J
  {
    {0,0,0,0},
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0}
  },
  // L
  {
    {0,0,0,0},
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0}
  }
};

static Tetromino generate_tetromino(int type) {
  Tetromino tet;
  tet.x = (FIELD_WIDTH / 2) - (TETROMINO_SIZE / 2);
  tet.y = -1;
  memcpy(tet.shape, tetromino_shapes[type], sizeof tet.shape);
  return tet;
}

Tetromino generate_random_tetromino(void) {
  static int initialized = 0;
  if (!initialized) {
    srand((unsigned)time(NULL));
    initialized = 1;
  }
  int type = rand() % 7;
  return generate_tetromino(type);
}

void tetris_init(TetrisGame *game) {
  memset(game->field, 0, sizeof(game->field));
  game->current = generate_random_tetromino();
  game->score = 0;
  game->high_score = 0;
  game->level = 1;
  game->state = STATE_INIT;
}

bool check_collision(TetrisGame *game, Tetromino *tet) {
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (tet->shape[y][x]) {
        int fx = tet->x + x;
        int fy = tet->y + y;
        if (fx < 0 || fx >= FIELD_WIDTH || fy >= FIELD_HEIGHT) return true;
        if (fy >= 0 && game->field[fy][fx]) return true;
      }
    }
  }
  return false;
}

void place_tetromino(TetrisGame *game) {
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (game->current.shape[y][x]) {
        int fx = game->current.x + x;
        int fy = game->current.y + y;
        if (fy >= 0 && fx >= 0 && fx < FIELD_WIDTH && fy < FIELD_HEIGHT) {
          game->field[fy][fx] = 1;
        }
      }
    }
  }
}

int clear_lines(TetrisGame *game) {
  int lines_cleared = 0;
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    bool full = true;
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (!game->field[y][x]) {
        full = false;
        break;
      }
    }
    if (full) {
      lines_cleared++;
      for (int row = y; row > 0; row--) {
        memcpy(game->field[row], game->field[row - 1], sizeof(game->field[row]));
      }
      memset(game->field[0], 0, sizeof(game->field[0]));
    }
  }
  return lines_cleared;
}

void update_level(TetrisGame *game) {
  int new_level = game->score / 600 + 1;
  if (new_level > game->level && new_level <= 10) {
    game->level = new_level;
  }
}

void tetris_input(TetrisGame *game, InputCommand cmd) {
  if (cmd == INPUT_NONE) return;

  if (cmd == INPUT_QUIT) {
    game->state = STATE_QUIT;
    return;
  }
  if (cmd == INPUT_PAUSE) {
    if (game->state == STATE_PLAYING) {
      game->state = STATE_PAUSED;
    } else if (game->state == STATE_PAUSED) {
      game->state = STATE_PLAYING;
    }
    return;
  }
  if (cmd == INPUT_START) {
    tetris_init(game);
    game->state = STATE_PLAYING;
    return;
  }
  if (game->state != STATE_PLAYING) return;

  Tetromino moved = game->current;

  switch (cmd) {
    case INPUT_LEFT: moved.x--; break;
    case INPUT_RIGHT: moved.x++; break;
    case INPUT_DOWN: moved.y++; break;
    case INPUT_DROP:
      while (!check_collision(game, &moved)) moved.y++;
      moved.y--;
      game->current = moved;
      tetris_update(game);
      return;
    case INPUT_ROTATE: {
      Tetromino rotated = moved;
      for (int y = 0; y < TETROMINO_SIZE; y++) {
        for (int x = 0; x < TETROMINO_SIZE; x++) {
          rotated.shape[y][x] = moved.shape[TETROMINO_SIZE - x - 1][y];
        }
      }
      if (!check_collision(game, &rotated)) {
        game->current = rotated;
      }
      return;
    }
    default: break;
  }

  if (!check_collision(game, &moved)) {
    game->current = moved;
  }
}

void tetris_update(TetrisGame *game) {
  if (game->state != STATE_PLAYING) return;

  Tetromino moved = game->current;
  moved.y++;

  if (!check_collision(game, &moved)) {
    game->current = moved;
  } else {
    place_tetromino(game);
    int lines = clear_lines(game);
    switch (lines) {
      case 1: game->score += 100; break;
      case 2: game->score += 300; break;
      case 3: game->score += 700; break;
      case 4: game->score += 1500; break;
      default: break;
    }
    if (game->score > game->high_score) {
      game->high_score = game->score;
    }
    update_level(game);
    game->current = generate_random_tetromino();

    if (check_collision(game, &game->current)) {
      game->state = STATE_GAME_OVER;
    }
  }
}
