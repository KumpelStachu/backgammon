#include "utils.h"

Vec2 *screen_size()
{
  static Vec2 size = {0, 0};

  getmaxyx(stdscr, size.y, size.x);

  return &size;
}

int msleep(long msec)
{
  struct timespec ts;
  int res;

  if (msec < 0)
  {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do
  {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}
