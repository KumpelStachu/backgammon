#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

#define GAMESTATE_FILE "gamestate.bgn"

#define TURN_WHITE 0
#define TURN_BLACK 1
#define TURN_PLAYER TURN_WHITE
#define TURN_COMPUTER TURN_BLACK

typedef struct Checkers
{
  uint8_t white;
  uint8_t black;
} Checkers;

typedef struct GameState
{
  Checkers board[24];
  Checkers bar;
  Checkers home;
  uint8_t turn;
  uint8_t dice[2];
  uint8_t moves[4];
  uint8_t selected_move;
  uint8_t selected_checker;
  bool show_history;
} GameState;

GameState *init_game_state();
GameState *load_game_state();
void save_game_state(GameState *game_state);
