#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CC "cc"
#define CFLAGS "-Wall",     \
               "-Wextra",   \
               "-std=c99",  \
               "-lncurses", \
               "-g",        \
               "-lm"
#define INPUT_FILES X("main"),                     \
                    X("backgammon"),               \
                    X("utils"),                    \
                    X("ref"),                      \
                    X("colors"),                   \
                    X("history"),                  \
                    X("game_state"),               \
                    X("leaderboard"),              \
                    X("screens/screens"),          \
                    X("screens/quit_screen"),      \
                    X("screens/main_screen"),      \
                    X("screens/game_screen"),      \
                    X("screens/winner_screen"),    \
                    X("screens/visualize_screen"), \
                    X("screens/leaderboard_screen")

void cmd_usage(char **argv)
{
  ERRO("Usage: %s [build|run|check]", argv[0]);
  exit(1);
}

void cmd_check()
{
  CMD("node", "tools/check.mjs"); //, "print"
}

void cmd_build()
{
  cmd_check();

#define X(name) "src/" name ".c"
  CMD(CC, CFLAGS, INPUT_FILES, "-o", "./build/backgammon");
#undef X
}

void cmd_run()
{
  cmd_build();

  INFO("./build/backgammon");
}

int main(int argc, char **argv)
{
  GO_REBUILD_URSELF(argc, argv);

  if (argc > 1)
  {
    if (!strcmp(argv[1], "build"))
      cmd_build();
    else if (!strcmp(argv[1], "run"))
      cmd_run();
    else if (!strcmp(argv[1], "check"))
      cmd_check();
    else
      cmd_usage(argv);
  }
  else
    cmd_usage(argv);

  return 0;
}