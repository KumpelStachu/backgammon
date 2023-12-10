#pragma once
#include "quit_screen.h"
#include "main_screen.h"
#include "game_screen.h"
#include "leaderboard_screen.h"
#include "winner_screen.h"
#include "visualize_screen.h"
#include "../utils.h"

#define SCREEN_LIST \
  X(0, quit)        \
  X(1, main)        \
  X(2, game)        \
  X(3, leaderboard) \
  X(4, winner)      \
  X(5, visualize)

#define X(id, name) void screen_##name();
SCREEN_LIST
#undef X