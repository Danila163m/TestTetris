#include <ncurses.h>
#include <unistd.h>
#include "tetris.h"

#define DELAY 50000

void draw_game(WINDOW *win, TetrisGame *game) {
  werase(win);
  box(win, 0, 0);

  // Игровое поле
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      if (game->field[y][x]) {
        mvwaddch(win, y + 1, x * 2 + 1, 'X');
        mvwaddch(win, y + 1, x * 2 + 2, 'X');
      }
    }
  }

  // Текущая фигура
  Tetromino *t = &game->current;
  for (int y = 0; y < TETROMINO_SIZE; ++y) {
    for (int x = 0; x < TETROMINO_SIZE; ++x) {
      if (t->shape[y][x]) {
        int draw_y = t->y + y + 1;
        int draw_x = (t->x + x) * 2 + 1;
        if (draw_y >= 1 && draw_y <= FIELD_HEIGHT) {
          mvwaddch(win, draw_y, draw_x, '#');
          mvwaddch(win, draw_y, draw_x + 1, '#');
        }
      }
    }
  }

  // Боковая панель
  mvwprintw(win, 1, FIELD_WIDTH * 2 + 3, "SCORE: %d", game->score);
  mvwprintw(win, 2, FIELD_WIDTH * 2 + 3, "LEVEL: %d", game->level);
  mvwprintw(win, 3, FIELD_WIDTH * 2 + 3, "HI-SCORE: %d", game->high_score);

  if (game->state == STATE_PAUSED) {
    mvwprintw(win, 10, FIELD_WIDTH * 2 + 3, "[PAUSED]");
  } else if (game->state == STATE_GAME_OVER) {
    mvwprintw(win, 10, FIELD_WIDTH * 2 + 3, "[GAME OVER]");
  }

  wrefresh(win);
}

InputCommand get_input(int ch) {
  switch (ch) {
    case 'q': return INPUT_QUIT;
    case 'p': return INPUT_PAUSE;
    case 's': return INPUT_START;
    case KEY_LEFT: return INPUT_LEFT;
    case KEY_RIGHT: return INPUT_RIGHT;
    case KEY_DOWN: return INPUT_DOWN;
    case ' ': return INPUT_DROP;
    case 'z': return INPUT_ROTATE;
    default: return INPUT_NONE;
  }
}

int main() {
  TetrisGame game;
  tetris_init(&game);

  initscr();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  timeout(0);

  WINDOW *win = newwin(FIELD_HEIGHT + 2, FIELD_WIDTH * 2 + 20, 1, 1);

  while (1) {
    int ch = getch();
    InputCommand cmd = get_input(ch);
    if (cmd == INPUT_QUIT) break;
    tetris_input(&game, cmd);

    if (game.state == STATE_PLAYING) {
      tetris_update(&game);
    }

    draw_game(win, &game);
    usleep(DELAY - (game.level - 1) * 2000);
  }

  endwin();
  return 0;
}


