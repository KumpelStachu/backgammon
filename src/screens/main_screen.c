#include "main_screen.h"

#define LOGO_HEIGHT 8
#define LOGO                                                                                        \
  X(" ____                   _                                                            ")        \
  X("|  _ \\                 | |                                                           ")       \
  X("| |_) |   __ _    ___  | | __   __ _    __ _   _ __ ___    _ __ ___     ___    _ __  ")        \
  X("|  _ <   / _` |  / __| | |/ /  / _` |  / _` | | '_ ` _ \\  | '_ ` _ \\   / _ \\  | '_ \\ ")    \
  X("| |_) | | (_| | | (__  |   <  | (_| | | (_| | | | | | | | | | | | | | | (_) | | | | |")        \
  X("|____/   \\__,_|  \\___| |_|\\_\\  \\__, |  \\__,_| |_| |_| |_| |_| |_| |_|  \\___/  |_| |_|") \
  X("                                __/ |                                                ")        \
  X("                               |___/                                                 ")

#define MENU_HEIGHT 17
#define MENU                                                           \
  X(0, "  ___         _   _              ")                            \
  X(0, " / __|___ _ _| |_(_)_ _ _  _ ___ ")                            \
  X(0, "| (__/ _ \\ ' \\  _| | ' \\ || / -_)")                         \
  X(0, " \\___\\___/_||_\\__|_|_||_\\_,_\\___|")                       \
                                                                       \
  X(1, " _  _                                  ")                      \
  X(1, "| \\| |_____ __ __   __ _ __ _ _ __  ___ ")                    \
  X(1, "| .` / -_) V  V /  / _` / _` | '  \\/ -_)")                    \
  X(1, "|_|\\_\\___|\\_/\\_/   \\__, \\__,_|_|_|_\\___|")              \
  X(1, "                  |___/                ")                      \
                                                                       \
  X(2, " _                _         _                      _ ")        \
  X(2, "| |   ___ __ _ __| |___ _ _| |__  ___  __ _ _ _ __| |")        \
  X(2, "| |__/ -_) _` / _` / -_) '_| '_ \\/ _ \\/ _` | '_/ _` |")      \
  X(2, "|____\\___\\__,_\\__,_\\___|_| |_.__/\\___/\\__,_|_| \\__,_|") \
                                                                       \
  X(3, "  ___       _ _   ")                                           \
  X(3, " / _ \\ _  _(_) |_ ")                                          \
  X(3, "| (_) | || | |  _|")                                           \
  X(3, " \\__\\_\\\\_,_|_|\\__|")

void render_logo()
{
  Vec2 size = *ref_screen_size();

  attron_RED_BLACK();

  int i = 0;
#define X(str) mvprintw((i++) + 1, (size.x - sizeof(str) - 1) / 2, str);
  LOGO
#undef X

  attroff_RED_BLACK();

  mvprintw(size.y - 1, size.x - 24, "Stanislaw Rzeszut 198318");
}

void render_menu()
{
  Vec2 size = *ref_screen_size();
  uint8_t *menu_ref = ref_menu_main();

  int i = 0;

#define X(id, str)                                                                                               \
  if (*menu_ref == id)                                                                                           \
    attron_YELLOW_BLACK();                                                                                       \
  mvprintw((size.y - LOGO_HEIGHT - MENU_HEIGHT) / 2 + LOGO_HEIGHT + (i++), (size.x - sizeof(str) - 1) / 2, str); \
  if (*menu_ref == id)                                                                                           \
    attroff_YELLOW_BLACK();
  MENU
#undef X
}

void handle_menu()
{
  uint8_t *menu_ref = ref_menu_main();

  switch (getch())
  {
  case KEY_UP:
    (*menu_ref) = (*menu_ref) > 0 ? (*menu_ref) - 1 : 3;
    break;
  case KEY_DOWN:
    (*menu_ref) = (*menu_ref) < 3 ? (*menu_ref) + 1 : 0;
    break;
  case 'c':
  case 'C':
    *menu_ref = 0;
    screen_game();
    break;
  case 'n':
  case 'N':
    *menu_ref = 1;
    screen_game();
    break;
  case 'L':
  case 'l':
    screen_leaderboard();
    break;
  case 'v':
  case 'V':
    screen_visualize();
    break;
  case 'q':
  case 'Q':
    screen_quit();
    break;
  case '\n':
    switch (*menu_ref)
    {
    case 0:
    case 1:
      screen_game();
      break;
    case 2:
      screen_leaderboard();
      break;
    case 3:
      screen_quit();
      break;
    }
    break;
  }
}

void main_screen()
{
  render_logo();
  render_menu();
  handle_menu();
}

void main_screen_init()
{
  uint8_t *menu_ref = ref_menu_main();
  *menu_ref = 0;
}