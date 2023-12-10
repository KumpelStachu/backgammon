#include "backgammon.h"

void init()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    // timeout(0);
    curs_set(0);
    start_color();
    init_colors();
    srand(time(NULL));
}

void backgammon()
{
    init();

    uint8_t prev_screen = 1;
    uint8_t *screen_ref = ref_screen();
    *screen_ref = prev_screen;

    Vec2 *screen_size_ref = ref_screen_size();

    while (true)
    {
        Vec2 s = *screen_size();
        screen_size_ref->x = s.x;
        screen_size_ref->y = s.y;

        switch (*screen_ref)
        {
#define X(id, name)                                   \
    case id:                                          \
        if (prev_screen != id && *screen_ref == id)   \
            (name##_screen_init(), prev_screen = id); \
        name##_screen();                              \
        break;
            SCREEN_LIST
#undef X
        }

        refresh();
        erase();
    }
}
