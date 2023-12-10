#pragma once
#include <math.h>
#include <unistd.h>
#include "screens.h"
#include "../game_state.h"
#include "../colors.h"
#include "../utils.h"
#include "../ref.h"

Vec2 margin();

void render_screen();
void update_screen();
void game_screen_init();
void game_screen();
