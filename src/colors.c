#include "colors.h"

#define X(id, fg, bg)                                   \
  void attron_##fg##_##bg() { attron(COLOR_PAIR(id)); } \
  void attroff_##fg##_##bg() { attroff(COLOR_PAIR(id)); }
COLORS_LIST
#undef X

void attron_BOLD() { attron(A_BOLD); }
void attroff_BOLD() { attroff(A_BOLD); }

void init_colors()
{
#define X(id, fg, bg) init_pair(id, COLOR_##fg, COLOR_##bg);
  COLORS_LIST
#undef X
}