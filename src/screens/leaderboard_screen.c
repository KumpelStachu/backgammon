#include "leaderboard_screen.h"

#define LOGO_HEIGHT 6
#define LOGO                                                                                    \
  X(" _                           _                 _                                  _ ")     \
  X("| |                         | |               | |                                | |")     \
  X("| |        ___    __ _    __| |   ___   _ __  | |__     ___     __ _   _ __    __| |")     \
  X("| |       / _ \\  / _` |  / _` |  / _ \\ | '__| | '_ \\   / _ \\   / _` | | '__|  / _` |") \
  X("| |____  |  __/ | (_| | | (_| | |  __/ | |    | |_) | | (_) | | (_| | | |    | (_| |")     \
  X("|______|  \\___|  \\__,_|  \\__,_|  \\___| |_|    |_.__/   \\___/   \\__,_| |_|     \\__,_|")

#define BUTTON_HEIGHT 5
#define BUTTON                             \
  X("  ___        _               _   ")   \
  X(" / __| ___  | |__  __ _  __ | |__")   \
  X("| (_ |/ _ \\ | '_ \\/ _` |/ _|| / /") \
  X(" \\___|\\___/ |_.__/\\__,_|\\__||_\\_\\")

void render_header()
{
  Vec2 size = *ref_screen_size();

  int i = 0;
#define X(str) mvprintw((i++) + 1, (size.x - sizeof(str) - 1) / 2, str);
  LOGO
#undef X
}

void render_leaderboard()
{
  LeaderboardEntry *leaderboard = load_leaderboard();
  Vec2 size = *ref_screen_size();

  for (uint8_t i = 0; i < 10; i++)
    mvprintw(LOGO_HEIGHT + (size.y - LEADERBOARD_SIZE - LOGO_HEIGHT - BUTTON_HEIGHT) / 2 + i + 1, (size.x - 26) / 2, "%2d. %-16s | %d", i + 1, leaderboard[i].score ? leaderboard[i].name : "---", leaderboard[i].score);

  free(leaderboard);
}

void render_button()
{
  Vec2 size = *ref_screen_size();

  attron_YELLOW_BLACK();

  int i = 0;
#define X(str) mvprintw(size.y - BUTTON_HEIGHT + (i++), (size.x - sizeof(str) - 1) / 2, str);
  BUTTON
#undef X

  attroff_YELLOW_BLACK();
}

void leaderboard_screen()
{
  render_header();
  render_leaderboard();
  render_button();

  char key = 0;
  while (key != '\n' && key != 'q' && key != 'Q')
    key = getch();

  screen_main();
}

void leaderboard_screen_init()
{
  // leaderboard_add("0123456789abcdef", 100);
  // leaderboard_add("Player 420", 420);
  // leaderboard_add("Player 69", 69);
  // leaderboard_add("Player 1", 1);
}