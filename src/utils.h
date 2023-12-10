#pragma once
#include <ncurses.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "vec2.h"

#define memzero(ptr, size) memset(ptr, 0, size)
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

Vec2 *screen_size();
int msleep(long msec);