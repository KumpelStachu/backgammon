#include "visualize_screen.h"

#define COMMAND_LIST \
  X("Q", "uit")      \
  X("[", "First")    \
  X("]", "Last")     \
  X("<", "Previous") \
  X(">", "Next")

void render_commands_visualize()
{
  Vec2 size = *ref_screen_size();
  Vec2 m = margin();
  m.y += 22;

  for (int i = m.x; i < size.x; i++)
    mvprintw(m.y, i + 10, " ");

  int i = 10;
#define X(first, rest)                    \
  {                                       \
    attron_YELLOW_BLACK();                \
    mvprintw(m.y, m.x + i, first);        \
    attroff_YELLOW_BLACK();               \
    mvprintw(m.y, m.x + i + 1, rest " "); \
    i += sizeof(rest) + 1;                \
  }

  COMMAND_LIST
#undef X
}

Move *parse_moves(char *data, uint8_t *count)
{
  (*count) = 1;
  for (uint8_t i = 0; data[i] != '\0'; i++)
    if (data[i] == ' ')
      (*count)++;

  Move *moves = malloc((*count) * sizeof(Move));

  uint8_t o = 0;
  for (uint8_t i = 0; i < *count; i++)
  {
    int from, to;
    char who;
    sscanf(data + o, "%d%c%d", &from, &who, &to);
    moves[i].who = who == 'W' ? TURN_WHITE : TURN_BLACK;
    moves[i].from = from;
    moves[i].to = to;
    o += 2 + (from > 9 ? 2 : 1) + (to > 9 ? 2 : 1);
  }

  return moves;
}

void perform_move(GameState *game_state, Move *move)
{
  uint8_t from = move->from;
  uint8_t to = move->to;

  if (move->who == TURN_WHITE)
  {
    if (to == HISTORY_BAR)
      game_state->bar.white++;
    else if (to == HISTORY_HOME)
      game_state->home.white++;
    else
      game_state->board[to - 1].white++;

    if (from == HISTORY_BAR)
      game_state->bar.white--;
    else
      game_state->board[from - 1].white--;
  }
  else
  {
    if (to == HISTORY_BAR)
      game_state->bar.black++;
    else if (to == HISTORY_HOME)
      game_state->home.black++;
    else
      game_state->board[to - 1].black++;

    if (from == HISTORY_BAR)
      game_state->bar.black--;
    else
      game_state->board[from - 1].black--;
  }
}

void update_game_state()
{
  GameState *prev_game_state = init_game_state();
  uint8_t index = *ref_history_index();
  HistoryEntry *head = ref_history();

  for (uint8_t i = 0; i < index; i++)
  {
    head = head->next;

    uint8_t count = 0;
    Move *moves = parse_moves(head->data, &count);

    for (uint8_t j = 0; j < count; j++)
    {
      perform_move(prev_game_state, &moves[j]);
    }
  }

  GameState *game_state = ref_game_state();
  memcpy(game_state, prev_game_state, sizeof(GameState));
  free(prev_game_state);

  game_state->selected_checker = -1;
  game_state->show_history = false; // DEBUG
}

void visualize_screen()
{
  uint8_t *index = ref_history_index();
  Vec2 m = margin();

  update_game_state();

  erase();
  render_screen();
  render_commands_visualize();
  mvprintw(m.y, m.x + 37, "Move %d", *index);
  refresh();

  uint8_t key = getch();
  switch (key)
  {
  case 'q':
  case 'Q':
    screen_main();
    return;
  case '[':
    *index = 0;
    break;
  case ']':
    *index = history_last(ref_history())->index;
    break;
  case ',': // <
    *index = max(0, *index - 1);
    break;
  case '.': // >
    *index = min(history_last(ref_history())->index, *index + 1);
    break;
  }
}

void visualize_screen_init()
{
  HistoryEntry *history = ref_history();
  HistoryEntry *loaded_history = history_load();
  *history = *loaded_history;

  *ref_history_index() = 0;
}
