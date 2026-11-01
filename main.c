#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

// x, y - cursor coords
void draw_map(int map[][9], int x, int y) {
  // v-slashes
  for (int xx = 0; xx < 4; ++xx) {
    for (int yy = 1; yy < 12; ++yy) {
      mvprintw(yy, xx * 10, "|");
    }
  }
  // h-slashes
  for (int yy = 0; yy < 4; ++yy) {
    for (int xx = 1; xx < (10 * 3); ++xx) {
      mvprintw(yy * 4, xx, "=");
    }
  }
  // cursor
  mvprintw(1 + y + (y / 3), 1 + x * 3 + (x / 3), "[ ]");
  // values
  for (int map_id = 0; map_id < 9; ++map_id) {
    for (int point = 0; point < 9; ++point) {
      if (map[map_id][point] != -1) {
        mvprintw(1 + map_id + (map_id / 3), 2 + point * 3 + (point / 3), "%i",
                 map[map_id][point]);
      }
    }
  }
}

_Bool check_to_place(int map[][9], int x, int y, int num) {
  for (int yy = 0; yy < 9; yy++) {
    if (map[yy][x] == num) {
      return 0;
    }
  }
  for (int xx = 0; xx < 9; xx++) {
    if (map[y][xx] == num) {
      return 0;
    }
  }
  for (int yy = 0; yy < 3; yy++) {
    for (int xx = 0; xx < 3; xx++) {
      // printf("[%i, %i] ", yy+(y/3)*3, xx+(x/3)*3);
      if (map[yy + (y / 3) * 3][xx + (x / 3) * 3] == num) {
        return 0;
      }
    }
  }
  return 1;
}

int recurse_xod(int map[][9], int solve[][9]) {
  int solutions = 0;

  int map_[9][9];
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      map_[y][x] = map[y][x];
    }
  }

  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      if (map[y][x] == -1) {
        for (int t = 1; t <= 9; t++) {
          if (check_to_place(map_, x, y, t)) {
            map_[y][x] = t;
            // draw_map(map_, x, y);
            solutions += recurse_xod(map_, solve);
            map_[y][x] = -1;
          }
        }
        return solutions;
      }
    }
  }
  // draw_map(map, -1, -1);
  // system("pause");

  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      solve[y][x] = map_[y][x];
    }
  }

  return 1;
}

void rand_map(int map[][9], int solve[][9]) {
  int map_[9][9];
  do {
    for (int y = 0; y < 9; y++) {
      for (int x = 0; x < 9; x++) {
        map_[y][x] = -1;
      }
    }

    // Just random insertions (by the rules of the game)
    for (int i = 0; i < 81; i++) {
      int x = rand() % 9, y = rand() % 9, t = rand() % 9 + 1;
      if (check_to_place(map_, x, y, t)) {
        map_[y][x] = t;
      }
    }

  } while (recurse_xod(map_, solve) != 1);

  // printf("Finded\n");

  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      map[y][x] = map_[y][x];
    }
  }
}

int main() {
  initscr();
  curs_set(0);
  cbreak();

  // Init Colors
  if (!has_colors()) {
    return 1;
  }
  start_color();
  use_default_colors();
  init_pair(1, COLOR_WHITE, -1);
  attron(COLOR_PAIR(1));

  int yMax, xMax;
  int map[9][9], right_choice[9][9];

  srand((int)time(NULL));

  // Init Map
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      map[y][x] = -1;
    }
  }
  rand_map(map, right_choice);

  int x = 0, y = 0, xp = 3;
  while (1) {
    getmaxyx(stdscr, yMax, xMax);
    attron(A_REVERSE);
    mvprintw(yMax - 1, xMax - 6, "XP: %i", xp);
    attroff(A_REVERSE);
    // draw_map(right_choice, x, y);
    draw_map(map, x, y);

    char tmp = getch();
    clear();
    if (tmp == 'a') {
      if (x - 1 >= 0) {
        x--;
      }
    } else if (tmp == 'd') {
      if (x + 1 < 9) {
        x++;
      }
    } else if (tmp == 'w') {
      if (y - 1 >= 0) {
        y--;
      }
    } else if (tmp == 's') {
      if (y + 1 < 9) {
        y++;
      }
    } else if ('1' <= tmp && tmp <= '9') {
      int t = tmp - '0';
      if (right_choice[y][x] == t) {
        map[y][x] = t;
      } else {
        xp--;
      }
    } else if (tmp == '0') {
      map[y][x] = -1;
    }
    if (tmp == '\n' || xp <= 0) {
      break;
    }
  }

  // Game Over window
  WINDOW *g_o = newwin(3, 20, yMax / 2, xMax / 2 - 10);
  box(g_o, 0, 0);
  mvwprintw(g_o, 1, 5, "Game Over");

  attroff(COLOR_PAIR(1));
  wgetch(g_o);
  delwin(g_o);
  endwin();
  return 0;
}
