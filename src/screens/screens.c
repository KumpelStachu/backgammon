#include "screens.h"

#define X(id, name) \
  void screen_##name() { *ref_screen() = id; }
SCREEN_LIST
#undef X