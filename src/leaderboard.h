#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define LEADERBOARD_SIZE 10
#define LEADERBOARD_FILE "leaderboard.bgn"
#define NAME_SIZE 16

typedef struct
{
  char name[NAME_SIZE + 1];
  uint16_t score;
} LeaderboardEntry;

LeaderboardEntry *load_leaderboard();
void save_leaderboard(LeaderboardEntry *leaderboard);
void leaderboard_add(char name[NAME_SIZE + 1], uint16_t score);