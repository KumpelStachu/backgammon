#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

#define HISTORY_FILE "history.bgn"
#define HISTORY_BAR 0
#define HISTORY_HOME 25

typedef struct Move
{
  uint8_t from;
  uint8_t to;
  uint8_t who;
} Move;

typedef struct HistoryEntry
{
  struct HistoryEntry *prev;
  struct HistoryEntry *next;
  uint16_t index;
  char *data;
} HistoryEntry;

HistoryEntry *history_init();
HistoryEntry *history_load();
void history_save(HistoryEntry *history);
void history_push(HistoryEntry *history, char *data);
void history_free(HistoryEntry *history);

HistoryEntry *history_first(HistoryEntry *history);
HistoryEntry *history_last(HistoryEntry *history);
HistoryEntry *history_previous(HistoryEntry *history);
HistoryEntry *history_next(HistoryEntry *history);
