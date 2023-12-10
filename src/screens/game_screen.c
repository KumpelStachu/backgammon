#include "game_screen.h"

#define COMMAND_LIST \
  X("Q", "uit")      \
  X("S", "ave")      \
  X("L", "oad")      \
  X("R", "oll")      \
  X("M", "ove")      \
  X("V", "isualize") \
  X("H", "istory")

Vec2 margin()
{
  Vec2 size = *ref_screen_size();
  uint8_t x = (size.x - 85) / 2;
  uint8_t y = (size.y - 23) / 2;

  return (Vec2){x, y};
}

uint8_t remaining_moves()
{
  GameState *game_state = ref_game_state();
  return (game_state->moves[0] != 0) + (game_state->moves[1] != 0) + (game_state->moves[2] != 0) + (game_state->moves[3] != 0);
}

bool can_home(uint8_t who)
{
  GameState *game_state = ref_game_state();

  if (who == TURN_PLAYER)
  {
    for (uint8_t i = 0; i < 18; i++)
      if (game_state->board[i].white)
        return false;
  }
  else
  {
    for (uint8_t i = 6; i < 24; i++)
      if (game_state->board[i].black)
        return false;
  }

  return true;
}

void history_add_moves(const Move *moves, uint8_t count)
{
  char *str = malloc(40);

  for (size_t i = 0; i < count; i++)
    sprintf(str, "%s%s%d%c%d", str, i > 0 ? " " : "", moves[i].from, moves[i].who == TURN_PLAYER ? 'W' : 'B', moves[i].to);

  history_push(ref_history(), str);
}

void render_board()
{
  Vec2 m = margin();

  for (uint8_t j = 0; j < 2; j++)
    for (uint8_t i = 0; i < 17; i++)
      mvprintw(m.y + 3 + i, m.x + 6 + j * 71, "::");

  for (uint8_t j = 0; j < 2; j++)
    for (uint8_t i = 0; i < 71; i++)
      mvprintw(m.y + 3 + j * 16, m.x + 7 + i, "=");

  for (uint8_t i = 0; i < 15; i++)
    mvprintw(m.y + 4 + i, m.x + 41, "|||");

  for (uint8_t l = 0; l < 2; l++)
    for (uint8_t k = 0; k < 2; k++)
      for (uint8_t j = 0; j < 6; j++)
        for (uint8_t i = 0; i < 6; i++)
          mvprintw(m.y + 4 + j + k * 9, m.x + 11 + i * 5 + l * 36, "..");

  for (uint8_t i = 0; i < 2; i++)
    mvprintw(m.y + 3 + i * 16, m.x + 42, "o");

  mvprintw(m.y, m.x + 6, "Backgammon PP");
  mvprintw(m.y + 11, m.x + 40, "[BAR]");
  mvprintw(m.y + 11, m.x + 80, "HOME");
}

void render_numbers()
{
  GameState *game_state = ref_game_state();
  Vec2 m = margin();

  for (uint8_t k = 0; k < 2; k++)
    for (uint8_t j = 0; j < 2; j++)
      for (uint8_t i = 0; i < 6; i++)
      {
        uint8_t num = (i + j * 6 + k * 12) * (k * -2 + 1) + ((k + 1) * 12 + 1) - k;
        if (num == game_state->selected_checker + 1 && game_state->selected_checker != (uint8_t)-1)
          attron_BLACK_MAGENTA();
        mvprintw(m.y + 2 + k * 18, m.x + 10 + i * 5 + j * 36, " %d ", num);
        attroff_BLACK_MAGENTA();
      }
}

void render_checkers()
{
  GameState *game_state = ref_game_state();
  Vec2 m = margin();
  m.x += 72;
  m.y += 18;

  for (uint8_t l = 0; l < 2; l++)
    for (uint8_t k = 0; k < 2; k++)
      for (uint8_t j = 0; j < 6; j++)
      {
        Checkers field = game_state->board[j + k * 6 + l * 12];
        uint8_t total = field.white + field.black;
        if (field.white)
          attron_BLACK_WHITE();
        else if (field.black)
          attron_BLACK_RED();
        for (uint8_t i = 0; i < min(total, 6); i++)
          mvprintw(m.y - i * (l * -2 + 1) - l * 14, m.x - (l ? 5 - j : j) * 5 - (l ? 1 - k : k) * 36, (total > 6) && (i == l * 5) ? "%02d" : "()", total - 5);
        attroff_BLACK_WHITE();
        attroff_BLACK_RED();
      }
}

void render_commands()
{
  GameState *game_state = ref_game_state();
  Vec2 m = margin();
  m.y += 22;

  int i = 10;
#define X(first, rest)                    \
  {                                       \
    attron_YELLOW_BLACK();                \
    mvprintw(m.y, m.x + i, first);        \
    attroff_YELLOW_BLACK();               \
    mvprintw(m.y, m.x + i + 1, rest " "); \
    i += sizeof(rest) + 1;                \
  }

  mvprintw(m.y, m.x, "Commands:");
  if (game_state->turn == TURN_PLAYER)
  {
    COMMAND_LIST
  }
  else
  {
    attron_RED_BLACK();
    mvprintw(m.y, m.x + i, "Waiting for computer...");
    attroff_RED_BLACK();
  }
#undef X
}

void render_dice()
{

  GameState *game_state = ref_game_state();
  Vec2 m = margin();

  if (game_state->dice[0] == 0 && game_state->dice[1] == 0)
    return;

  mvprintw(m.y, m.x + 32, " Dice:         ");
  attron_BLACK_WHITE();
  for (uint8_t i = 0; i < 2; i++)
    mvprintw(m.y, m.x + 39 + i * 4, " %d ", game_state->dice[i]);
  attroff_BLACK_WHITE();
}

void render_moves()
{
  bool selecting_move = *ref_selecting_move();
  GameState *game_state = ref_game_state();
  Vec2 m = margin();

  if (game_state->moves[0] == 0)
    return;

  uint8_t moves = (game_state->moves[0] != 0) + (game_state->moves[1] != 0) + (game_state->moves[2] != 0) + (game_state->moves[3] != 0);

  mvprintw(m.y, m.x + 31, " Moves:                ");
  for (uint8_t i = 0; i < moves; i++)
  {
    if (selecting_move && game_state->selected_move == i)
      attron_BLACK_YELLOW();
    else
      attron_BLACK_WHITE();

    mvprintw(m.y, m.x + 39 + i * 4, " %d ", game_state->moves[i]);
  }
  attroff_BLACK_YELLOW();
  attroff_BLACK_WHITE();
}

void render_score_player()
{
  GameState *game_state = ref_game_state();
  Vec2 m = margin();
  uint8_t score = 0;

  for (uint8_t i = 0; i < 24; i++)
  {
    Checkers field = game_state->board[i];
    score += field.white * (24 - i);
  }

  attron_BLACK_WHITE();
  mvprintw(m.y + 4, m.x, " %d ", score);
  mvprintw(m.y + 16, m.x + 41, " %-2d", game_state->bar.white);
  if (can_home(TURN_PLAYER) || can_home(TURN_COMPUTER))
    mvprintw(m.y + 9, m.x + 80, " %d ", game_state->home.white);
  attroff_BLACK_WHITE();
}

void render_score_computer()
{
  GameState *game_state = ref_game_state();
  Vec2 m = margin();
  uint8_t score = 0;

  for (uint8_t i = 0; i < 24; i++)
  {
    Checkers field = game_state->board[i];
    score += field.black * (i + 1);
  }

  attron_BLACK_RED();
  mvprintw(m.y + 18, m.x, " %d ", score);
  mvprintw(m.y + 6, m.x + 41, " %-2d", game_state->bar.black);
  if (can_home(TURN_COMPUTER) || can_home(TURN_PLAYER))
    mvprintw(m.y + 13, m.x + 80, " %d ", game_state->home.black);
  attroff_BLACK_RED();
}

void render_score()
{
  render_score_player();
  render_score_computer();
}

void roll_dice()
{
  bool *selecting_move = ref_selecting_move();
  GameState *game_state = ref_game_state();
  game_state->selected_move = 0;

  *selecting_move = false;

  for (uint16_t i = 0; i < 32; i++)
  {
    game_state->dice[0] = arc4random() % 6 + 1;
    game_state->dice[1] = arc4random() % 6 + 1;

    attron_WHITE_MAGENTA();
    render_dice();
    refresh();
    delay_output(3 * pow(1.15, i));
  }
  attroff_WHITE_MAGENTA();

  memzero(game_state->moves, sizeof(game_state->moves));

  uint8_t *moves = game_state->moves;
  uint8_t *dice = game_state->dice;

  moves[0] = dice[0];
  moves[1] = dice[1];
  if (dice[0] == dice[1])
  {
    moves[2] = dice[0];
    moves[3] = dice[1];
  }
}

void select_move()
{
  bool *selecting_move = ref_selecting_move();
  GameState *game_state = ref_game_state();
  uint8_t moves_left = remaining_moves();
  uint16_t key = 0;

  if (moves_left == 0)
    return;

  *selecting_move = true;

  while (key != '\n')
  {
    render_moves();

    key = getch();

    switch (key)
    {
    case KEY_LEFT:
      game_state->selected_move = (game_state->selected_move + 3) % moves_left;
      break;
    case KEY_RIGHT:
      game_state->selected_move = (game_state->selected_move + 1) % moves_left;
      break;
    }
  }
}

void remove_move()
{
  bool *selecting_move = ref_selecting_move();
  GameState *game_state = ref_game_state();
  uint8_t moves[4];

  memcpy(moves, game_state->moves, sizeof(moves));
  memzero(game_state->moves, sizeof(moves));

  uint8_t j = 0;
  for (uint8_t i = 0; i < 4; i++)
    if (i != game_state->selected_move)
      game_state->moves[j++] = moves[i];

  game_state->selected_move = 0;
  *selecting_move = false;
  update_screen();
}

void render_history()
{
  GameState *game_state = ref_game_state();
  if (!game_state->show_history)
    return;

  HistoryEntry *history = history_last(ref_history());
  Vec2 size = *ref_screen_size();

  attron_BLACK_YELLOW();
  mvprintw(0, 0, "History");
  attroff_BLACK_YELLOW();

  for (uint8_t i = 1; i < size.y && history->prev != NULL; i++)
  {
    mvprintw(i, 0, "%d. %s", history->index, history->data);

    history = history->prev;
  }
}

bool computer_handle_bar()
{
  GameState *gs = ref_game_state();
  Checkers *board = gs->board;
  Checkers *bar = &gs->bar;
  uint8_t *moves = gs->moves;

  if (bar->black == 0)
    return false;

  for (uint8_t i = 0; i < remaining_moves(); i++)
  {
    uint8_t move = 24 - moves[i], white = board[move].white;

    if (move == 24 || white > 1)
      continue;

    if (white == 1)
    {
      bar->white++;
      board[move].white--;
    }

    bar->black--;
    board[move].black++;

    gs->selected_move = i;
    remove_move();

    Move moves[2] = {{HISTORY_BAR, move + 1, TURN_COMPUTER}, {HISTORY_BAR, move + 1, TURN_PLAYER}};
    history_add_moves(moves, 1 + white);

    return true;
  }

  return false;
}

bool computer_handle_violence()
{
  GameState *game_state = ref_game_state();
  Checkers *board = game_state->board;

  for (uint8_t i = 0; i < remaining_moves(); i++)
  {
    uint8_t move = game_state->moves[i];

    if (move == 0)
      continue;

    for (uint8_t j = 23; j >= move; j--)
    {
      if (board[j].black == 0)
        continue;

      if (board[j - move].white != 1)
        continue;

      board[j - move].white--;
      game_state->bar.white++;

      board[j - move].black++;
      board[j].black--;

      game_state->selected_move = i;
      remove_move();

      Move moves[2] = {{j + 1, j - move + 1, TURN_COMPUTER}, {j - move + 1, HISTORY_BAR, TURN_PLAYER}};
      history_add_moves(moves, 2);

      return true;
    }
  }

  return false;
}

bool computer_handle_home()
{
  if (!can_home(TURN_COMPUTER))
    return false;

  GameState *game_state = ref_game_state();

  for (uint8_t i = 0; i < remaining_moves(); i++)
  {
    uint8_t move = game_state->moves[i];

    if (move == 0)
      continue;

    for (uint8_t j = 0; j < move; j++)
    {
      if (game_state->board[j].black == 0)
        continue;

      game_state->board[j].black--;
      game_state->home.black++;

      game_state->selected_move = i;
      remove_move();

      Move moves[1] = {{j + 1, HISTORY_HOME, TURN_COMPUTER}};
      history_add_moves(moves, 1);

      return true;
    }
  }

  return false;
}

bool computer_handle_move()
{
  GameState *gs = ref_game_state();
  Checkers *board = gs->board;

  for (uint8_t i = 0; i < remaining_moves(); i++)
  {
    uint8_t move = gs->moves[i];

    if (move == 0)
      continue;

    for (uint8_t j = 23; j >= 0; j--)
    {
      uint8_t white = board[j - move].white;

      if (board[j].black == 0 || white > 1)
        continue;

      if (white == 1)
      {
        board[j - move].white--;
        gs->bar.white++;
      }

      board[j - move].black++;
      board[j].black--;

      gs->selected_move = i;
      remove_move();

      Move moves[2] = {{j + 1, j - move + 1, TURN_COMPUTER}, {j - move + 1, HISTORY_BAR, TURN_PLAYER}};
      history_add_moves(moves, 1 + white);

      return true;
    }
  }

  return false;
}

void computer_turn()
{
  GameState *game_state = ref_game_state();
  bool *selecting_move = ref_selecting_move();

  roll_dice();
  msleep(500);

  while (true)
  {
    *selecting_move = true;
    update_screen();
    msleep(1000);

    if (computer_handle_bar())
      ;
    else if (computer_handle_violence())
      ;
    else if (computer_handle_home())
      ;
    else if (computer_handle_move())
      ;
    else
      break;
  }

  for (uint8_t i = 0; i < 2; i++)
    game_state->dice[i] = 0;

  for (uint8_t i = 0; i < 4; i++)
    game_state->moves[i] = 0;

  game_state->turn = TURN_PLAYER;

  update_screen();
}

void player_select_checker(uint8_t *legal_moves)
{
  GameState *game_state = ref_game_state();
  uint8_t total = 0, m = 0;
  int key = 0;

  while (legal_moves[total] != (uint8_t)-1)
    total++;

  while (key != '\n')
  {
    switch (key)
    {
    case KEY_LEFT:
      m = (m + 1) % total;
      break;
    case KEY_RIGHT:
      m = (m - 1 + total) % total;
      break;
    }
    game_state->selected_checker = legal_moves[m];

    update_screen();

    key = getch();
  }

  game_state->selected_checker = legal_moves[m];
}

bool player_handle_bar()
{
  GameState *game_state = ref_game_state();
  uint8_t move = game_state->moves[game_state->selected_move];

  uint8_t black = game_state->board[move - 1].black;

  if (game_state->bar.white == 0)
    return false;

  if (black > 1)
    return false;

  if (black == 1)
  {
    game_state->bar.black++;
    game_state->board[move - 1].black--;
  }

  game_state->bar.white--;
  game_state->board[move - 1].white++;

  Move moves[2] = {{HISTORY_BAR, move - 1 + 1, TURN_PLAYER}, {HISTORY_BAR, move - 1 + 1, TURN_COMPUTER}};
  history_add_moves(moves, 1 + black);

  return true;
}

uint8_t *player_violence_moves()
{
  GameState *game_state = ref_game_state();
  uint8_t move = game_state->moves[game_state->selected_move];
  uint8_t *legal_moves = malloc(24 * sizeof(uint8_t));
  uint8_t i = 0;

  memset(legal_moves, -1, 24 * sizeof(uint8_t));

  for (uint8_t j = 0; j < 24 - move; j++)
    if (game_state->board[j].white && game_state->board[j + move].black == 1)
      legal_moves[i++] = j;

  return legal_moves;
}

bool player_handle_violence()
{
  GameState *gs = ref_game_state();

  if (gs->bar.white > 0)
    return false;

  uint8_t move = gs->moves[gs->selected_move];
  uint8_t *legal_moves = player_violence_moves();

  if (legal_moves[0] == (uint8_t)-1)
  {
    free(legal_moves);
    return false;
  }

  player_select_checker(legal_moves);

  uint8_t sc = gs->selected_checker;
  Checkers *board = gs->board;

  board[sc + move].white++;
  board[sc].white--;

  board[sc + move].black--;
  gs->bar.black++;

  Move moves[2] = {{sc + 1, sc + move + 1, TURN_PLAYER}, {sc + move + 1, HISTORY_BAR, TURN_COMPUTER}};
  history_add_moves(moves, 2);

  free(legal_moves);
  return true;
}

uint8_t *player_home_moves()
{
  GameState *game_state = ref_game_state();
  uint8_t move = game_state->moves[game_state->selected_move];
  uint8_t *legal_moves = malloc(24 * sizeof(uint8_t));
  uint8_t i = 0;

  memset(legal_moves, -1, 24 * sizeof(uint8_t));

  for (uint8_t j = 24 - move; j < 24; j++)
    if (game_state->board[j].white)
      legal_moves[i++] = j;

  return legal_moves;
}

bool player_handle_home()
{
  GameState *game_state = ref_game_state();

  if (!can_home(TURN_PLAYER))
    return false;

  uint8_t *legal_moves = player_home_moves();

  if (legal_moves[0] == (uint8_t)-1)
  {
    free(legal_moves);
    return false;
  }

  player_select_checker(legal_moves);

  uint8_t checker = game_state->selected_checker;

  game_state->board[checker].white--;
  game_state->home.white++;

  Move moves[1] = {{checker + 1, HISTORY_HOME, TURN_PLAYER}};
  history_add_moves(moves, 1);

  free(legal_moves);
  return true;
}

uint8_t *player_move_moves()
{
  GameState *game_state = ref_game_state();
  uint8_t move = game_state->moves[game_state->selected_move];
  uint8_t *legal_moves = malloc(24 * sizeof(uint8_t));
  uint8_t i = 0;

  memset(legal_moves, -1, 24 * sizeof(uint8_t));

  for (uint8_t j = 0; j < 24 - move; j++)
    if (game_state->board[j].white && game_state->board[j + move].black == 0)
      legal_moves[i++] = j;

  return legal_moves;
}

bool player_handle_move()
{
  GameState *game_state = ref_game_state();

  if (game_state->bar.white > 0)
    return false;

  uint8_t move = game_state->moves[game_state->selected_move];
  uint8_t *legal_moves = player_move_moves();

  if (legal_moves[0] == (uint8_t)-1)
  {
    free(legal_moves);
    return false;
  }

  player_select_checker(legal_moves);

  uint8_t checker = game_state->selected_checker;

  game_state->board[checker + move].white++;
  game_state->board[checker].white--;

  Move moves[1] = {{checker + 1, checker + move + 1, TURN_PLAYER}};
  history_add_moves(moves, 1);

  free(legal_moves);
  return true;
}

void player_move()
{
  GameState *game_state = ref_game_state();

  select_move();

  if (player_handle_bar())
    ;
  else if (player_handle_home())
    ;
  else if (player_handle_violence())
    ;
  else
    player_handle_move();

  remove_move();

  if (remaining_moves() == 0)
    game_state->turn = TURN_COMPUTER;
}

void command_quit()
{
  // history_free(ref_history());
  screen_main();
}

void command_save()
{
  save_game_state(ref_game_state());
  history_save(ref_history());
}

void command_load()
{
  GameState *game_state = ref_game_state();
  GameState *loaded_game_state = load_game_state();
  memcpy(game_state, loaded_game_state, sizeof(GameState));
  free(loaded_game_state);

  HistoryEntry *loaded_history = history_load();
  HistoryEntry *history = ref_history();
  // history_free(history);
  *history = *loaded_history;
}

void command_roll()
{
  GameState *game_state = ref_game_state();
  if (game_state->moves[0] == 0)
    roll_dice();
}

void command_move()
{
  GameState *game_state = ref_game_state();
  if (game_state->moves[0] != 0)
    player_move();
}

void command_history()
{
  GameState *game_state = ref_game_state();
  game_state->show_history = !game_state->show_history;
}

void command_visualize()
{
  screen_visualize();
}

void handle_input()
{
  switch (getch())
  {
  case 'q':
  case 'Q':
    command_quit();
    break;
  case 's':
  case 'S':
    command_save();
    break;
  case 'l':
  case 'L':
    command_load();
    break;
  case 'r':
  case 'R':
    command_roll();
    break;
  case 'm':
  case 'M':
    command_move();
    break;
  case 'h':
  case 'H':
    command_history();
    break;
  case 'v':
  case 'V':
    command_visualize();
    break;
  }
}

void check_winner()
{
  GameState *game_state = ref_game_state();
  Checkers *board = game_state->board, bar = game_state->bar, home = game_state->home;
  uint8_t plock = 0, clock = 0;

  for (uint8_t i = 18; i < 24; i++)
    if (board[i].white > 1)
      plock++;

  for (uint8_t i = 0; i < 6; i++)
    if (board[i].black > 1)
      clock++;

  if (home.white == 15 || home.black == 15 || (plock == 6 && bar.black > 0) || (clock == 6 && bar.white > 0))
  {
    int8_t *points = ref_points();
    *points = home.white > home.black
                  ? (home.black > 0 || plock == 6
                         ? 1
                         : (bar.black == 0 ? 2 : 3))
                  : (home.white > 0 || clock == 6
                         ? -1
                         : (bar.white == 0 ? -2 : -3));

    screen_winner();
  }
}

void render_screen()
{
  render_history();
  render_board();
  render_numbers();
  render_checkers();
  render_commands();
  render_score();
  render_moves();
}

void update_screen()
{
  erase();
  render_screen();
  refresh();
}

void game_screen()
{
  GameState *game_state = ref_game_state();
  game_state->selected_checker = -1;

  update_screen();

  if (game_state->turn == TURN_COMPUTER)
    computer_turn();
  else
    handle_input();

  check_winner();
}

void game_screen_init()
{
  GameState *game_state = ref_game_state();
  HistoryEntry *history = ref_history();
  bool new_game = *ref_menu_main();

  GameState *source_state = new_game ? init_game_state() : load_game_state();

  memcpy(game_state, source_state, sizeof(GameState));
  free(source_state);

  HistoryEntry *source_history = new_game ? history_init() : history_load();
  *history = *source_history;

  int8_t *points = ref_points();
  *points = 0;
}
