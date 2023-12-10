#include "winner_screen.h"

#define BUTTON_HEIGHT 5
#define BUTTON                             \
  X("  ___        _               _   ")   \
  X(" / __| ___  | |__  __ _  __ | |__")   \
  X("| (_ |/ _ \\ | '_ \\/ _` |/ _|| / /") \
  X(" \\___|\\___/ |_.__/\\__,_|\\__||_\\_\\")

#define COMPUTER_WON_HEIGHT 8
#define COMPUTER_WON                                                                     \
  X("  _____                            _                                  _ ")          \
  X(" / ____|                          | |                                | |")          \
  X("| |     ___  _ __ ___  _ __  _   _| |_ ___ _ __  __      _____  _ __ | |")          \
  X("| |    / _ \\| '_ ` _ \\| '_ \\| | | | __/ _ \\ '__| \\ \\ /\\ / / _ \\| '_ \\| |") \
  X("| |___| (_) | | | | | | |_) | |_| | ||  __/ |     \\ V  V / (_) | | | |_|")         \
  X(" \\_____\\___/|_| |_| |_| .__/ \\__,_|\\__\\___|_|      \\_/\\_/ \\___/|_| |_(_)")  \
  X("                      | |                                               ")          \
  X("                      |_|                                               ")

#define PLAYER_WON_HEIGHT 6
#define PLAYER_WON                                       \
  X("__     __                               _ ")        \
  X("\\ \\   / /                              | |")      \
  X(" \\ \\_/ /__  _   _  __      _____  _ __ | |")      \
  X("  \\   / _ \\| | | | \\ \\ /\\ / / _ \\| '_ \\| |") \
  X("   | | (_) | |_| |  \\ V  V / (_) | | | |_|")       \
  X("   |_|\\___/ \\__,_|   \\_/\\_/ \\___/|_| |_(_)")

void print_button()
{
  Vec2 size = *ref_screen_size();

  attron_YELLOW_BLACK();

  int i = 0;
#define X(str) mvprintw(size.y - BUTTON_HEIGHT + (i++), (size.x - sizeof(str) - 1) / 2, str);
  BUTTON
#undef X

  attroff_YELLOW_BLACK();
}

void print_winner()
{
  Vec2 size = *ref_screen_size();
  int8_t points = *ref_points();
  bool player_won = points > 0;

  if (player_won)
    attron_GREEN_BLACK();
  else
    attron_RED_BLACK();

  int i = 0;
#define X(str) mvprintw((size.y - BUTTON_HEIGHT - (player_won ? PLAYER_WON_HEIGHT : COMPUTER_WON_HEIGHT)) / 2 + (i++), (size.x - sizeof(str) - 1) / 2, str);
  if (player_won)
  {
    PLAYER_WON

    char name[17];
    getlogin_r(name, 17);
    name[16] = '\0';

    leaderboard_add(name, points);
  }
  else
  {
    COMPUTER_WON
    leaderboard_add("Computer", -points);
  }
#undef X

  attroff_GREEN_BLACK();
  attroff_RED_BLACK();
}

void winner_screen()
{
  print_button();
  print_winner();

  char key = 0;
  while (key != '\n' && key != 'q' && key != 'Q')
    key = getch();

  int8_t *points = ref_points();
  *points = 0;

  screen_main();
}

void winner_screen_init()
{
}