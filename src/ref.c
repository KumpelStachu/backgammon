#include "ref.h"

void *ref(uint8_t i)
{
  static void *refs[REF_COUNT] = {0};

  if (refs[i] == NULL)
  {
#define X(id, name, type) refs[id] = malloc(sizeof(type));
    REF_LIST
#undef X
  }

  return refs[i];
}

#define X(id, name, type) \
  type *ref_##name() { return ref(id); }
REF_LIST
#undef X