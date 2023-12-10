#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "vec2.h"
#include "game_state.h"
#include "history.h"

#define REF_COUNT 8

#define REF_LIST                \
    X(0, screen, uint8_t)       \
    X(1, menu_main, uint8_t)    \
    X(2, screen_size, Vec2)     \
    X(3, game_state, GameState) \
    X(4, points, int8_t)        \
    X(5, selecting_move, bool)  \
    X(6, history, HistoryEntry) \
    X(7, history_index, uint8_t)

void *ref(uint8_t i);

#define X(id, name, type) \
    type *ref_##name();
REF_LIST
#undef X