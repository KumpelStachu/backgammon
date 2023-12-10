#include "game_state.h"

GameState *init_game_state()
{
  GameState *game_state = malloc(sizeof(GameState));
  memzero(game_state, sizeof(GameState));

  game_state->board[0].white = 2;
  game_state->board[5].black = 5;
  game_state->board[7].black = 3;
  game_state->board[11].white = 5;
  game_state->board[12].black = 5;
  game_state->board[16].white = 3;
  game_state->board[18].white = 5;
  game_state->board[23].black = 2;

  return game_state;
}

GameState *load_game_state()
{
  FILE *file = fopen(GAMESTATE_FILE, "rb");

  if (!file)
    return init_game_state();

  GameState *game_state = malloc(sizeof(GameState));

  fread(game_state, sizeof(GameState), 1, file);
  fclose(file);

  return game_state;
}

void save_game_state(GameState *game_state)
{
  FILE *file = fopen(GAMESTATE_FILE, "wb");

  fwrite(game_state, sizeof(GameState), 1, file);
  fclose(file);
}
