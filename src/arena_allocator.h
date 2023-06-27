#ifndef REGION_ALLOCATOR_H
#define REGION_ALLOCATOR_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct region_t region_t;
typedef struct region_t {
  region_t *next;
  size_t count;
  size_t cap;
  uintptr_t data[];
} region_t;

typedef struct arena_t {
  size_t reg_count;
  region_t *beg;
  region_t *end;
} arena_t;

#define REGION_DEFAULT_SIZE (8 * 1024)

inline region_t *region_create(size_t cap) {
  size_t size = sizeof(region_t) + sizeof(uintptr_t) * cap;
  region_t *r = (region_t *)malloc(size);
  r->cap = cap;
  r->count = 0;
  r->next = NULL;
  return r;
}

inline void region_free(region_t *r) {
  assert(r);
  free(r);
}

inline void *arena_alloc(arena_t *arena, size_t size_bytes) {
  assert(arena);

  size_t size = (size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

  if (arena->end == NULL) {
    assert(arena->beg == NULL);
    size_t cap = REGION_DEFAULT_SIZE;
    if (cap < size)
      cap = size;
    arena->end = region_create(cap);
    arena->beg = arena->end;
    arena->reg_count++;
  }

  if (arena->end->count + size >= arena->end->cap) {
    assert(arena->end->next == NULL);
    size_t cap = REGION_DEFAULT_SIZE;
    if (cap < size)
      cap = size;
    arena->end->next = region_create(cap);
    arena->end = arena->end->next;
    arena->reg_count++;
  }

  void *ptr = (void *)(arena->end->data + arena->end->count);
  arena->end->count += size;
  return ptr;
}

inline void arena_free(arena_t *arena) {
  region_t *r = arena->beg;
  while (r) {
    region_t *cur = r;
    r = r->next;
    region_free(cur);
  }
  arena->beg = NULL;
  arena->end = NULL;
}

#endif
