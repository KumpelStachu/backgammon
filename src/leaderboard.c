#include "leaderboard.h"

LeaderboardEntry *load_leaderboard()
{
  FILE *file = fopen(LEADERBOARD_FILE, "rb");

  LeaderboardEntry *leaderboard = malloc(sizeof(LeaderboardEntry) * LEADERBOARD_SIZE);
  memzero(leaderboard, sizeof(LeaderboardEntry) * LEADERBOARD_SIZE);

  if (file != NULL)
  {
    fread(leaderboard, sizeof(LeaderboardEntry), LEADERBOARD_SIZE, file);
    fclose(file);
  }

  for (uint8_t i = 0; i < LEADERBOARD_SIZE; i++)
    leaderboard[i].name[NAME_SIZE] = '\0';

  return leaderboard;
}

void save_leaderboard(LeaderboardEntry *leaderboard)
{
  FILE *file = fopen(LEADERBOARD_FILE, "wb");

  for (uint8_t i = 0; i < LEADERBOARD_SIZE; i++)
    leaderboard[i].name[NAME_SIZE] = '\0';

  fwrite(leaderboard, sizeof(LeaderboardEntry), LEADERBOARD_SIZE, file);
  fclose(file);
}

bool leaderboard_update(LeaderboardEntry *leaderboard, char name[NAME_SIZE + 1], uint16_t score)
{
  for (uint8_t i = 0; i < LEADERBOARD_SIZE; i++)
  {
    if (strcmp(leaderboard[i].name, name) == 0)
    {
      leaderboard[i].score += score;

      for (uint8_t j = i; j > 0; j--)
      {
        if (leaderboard[j].score > leaderboard[j - 1].score)
        {
          LeaderboardEntry tmp = leaderboard[j];
          leaderboard[j] = leaderboard[j - 1];
          leaderboard[j - 1] = tmp;
        }
        else
          break;
      }

      return true;
    }
  }

  return false;
}

void leaderboard_add(char name[NAME_SIZE + 1], uint16_t score)
{
  LeaderboardEntry *leaderboard = load_leaderboard();

  if (leaderboard_update(leaderboard, name, score))
    return save_leaderboard(leaderboard);

  for (uint8_t i = 0; i < LEADERBOARD_SIZE; i++)
  {
    if (leaderboard[i].score < score)
    {
      for (uint8_t j = LEADERBOARD_SIZE - 1; j > i; j--)
        leaderboard[j] = leaderboard[j - 1];

      strcpy(leaderboard[i].name, name);
      leaderboard[i].score = score;

      break;
    }
  }

  save_leaderboard(leaderboard);
}