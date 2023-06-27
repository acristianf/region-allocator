#include "arena_allocator.h"

typedef struct node_t {
  size_t number;
  struct node_t *next;
} node_t;

int main() {
  arena_t arena = {0};
  node_t *head = (node_t *)arena_alloc(&arena, sizeof(node_t));
  head->next = NULL;
  head->number = 0;
  for (int i = 0; i < 1000; i++) {
    node_t *cur = (node_t *)arena_alloc(&arena, sizeof(node_t));
    cur->number = i + 1;
    cur->next = NULL;
    head->next = cur;
    head = cur;
  }
  arena_free(&arena);
  return 0;
}
