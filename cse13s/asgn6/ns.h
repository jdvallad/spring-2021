#ifndef __NS_H__
#define __NS_H__

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct ns ns;

ns *ns_create(uint32_t capacity);

void ns_delete(ns **s);

uint32_t ns_size(ns *s);

bool ns_empty(ns *s);

bool ns_full(ns *s);

bool ns_push(ns *s, uint32_t x);

bool ns_pop(ns *s, uint32_t *x);

bool ns_peek(ns *s, uint32_t *x);

void ns_copy(ns *dst, ns *src);

void ns_print(ns *s);

#endif
