#include "history.h"

HistoryEntry *history_init()
{
  HistoryEntry *history = malloc(sizeof(HistoryEntry));
  memzero(history, sizeof(HistoryEntry));

  return history;
}

HistoryEntry *history_load()
{
  FILE *file = fopen(HISTORY_FILE, "r");
  HistoryEntry *history = history_init();

  if (!file)
    return history;

  char c;
  while (c = fgetc(file), c != '\n' && c != EOF)
    ;

  while (!feof(file))
  {
    char *data = malloc(40);
    fgets(data, 40, file);
    data[strlen(data) - 1] = '\0';
    history_push(history, data);
  }

  fclose(file);

  history->data = NULL;

  HistoryEntry *last = history_last(history);
  if (last->prev != NULL)
  {
    last->prev->next = NULL;
    free(last);
  }

  return history;
}

void history_save(HistoryEntry *history)
{
  FILE *file = fopen(HISTORY_FILE, "w");

  HistoryEntry *head = history_first(history);

  while (head != NULL)
  {
    fprintf(file, "%s\n", head->data);

    head = head->next;
  }

  fclose(file);
}

void history_push(HistoryEntry *history, char *data)
{
  HistoryEntry *entry = malloc(sizeof(HistoryEntry));
  memzero(entry, sizeof(HistoryEntry));

  while (history->next != NULL)
    history = history->next;

  // char *new_data = malloc(strlen(data) + 1);
  // sprintf(new_data, "%s", data);
  // new_data[strlen(data)] = '\0';

  history->next = entry;
  entry->prev = history;
  entry->data = data;
  entry->index = history->index + 1;
}

void history_free(HistoryEntry *history)
{
  HistoryEntry *node = history;
  while (node)
  {
    free(node->data);

    HistoryEntry *next = node->next;
    free(node);
    node = next;
  }
}

HistoryEntry *history_first(HistoryEntry *history)
{
  while (history->prev != NULL)
    history = history->prev;

  return history;
}

HistoryEntry *history_last(HistoryEntry *history)
{
  while (history->next != NULL)
    history = history->next;

  return history;
}

HistoryEntry *history_previous(HistoryEntry *history)
{
  if (history->prev == NULL)
    return history;

  return history->prev;
}

HistoryEntry *history_next(HistoryEntry *history)
{
  if (history->next == NULL)
    return history;

  return history->next;
}