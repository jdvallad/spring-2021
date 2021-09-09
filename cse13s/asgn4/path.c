#include "path.h"

#include "stack.c"
#include "vertices.h"

// Structure definition given in Assignment 4 PDF.
struct Path {
    Stack *vertices;
    uint32_t length;
};

// Path Constructor, allocates proper amount of memory to store a path.
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    if (!p) {
        free(p);
        p = NULL;
        return p;
    }
    p->vertices = stack_create(VERTICES);
    p->length = 0;
    return p;
}

// Path destructor, frees memory when path is no longer needed.
void path_delete(Path **p) {
    if (*p) {
        stack_delete(&(*p)->vertices);
        free(*p);
        *p = NULL;
    }
    return;
}

// adds vertex to path if possible, increments path length if possible. Returns whether vertex
// was successfully added.
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    if (v >= G->vertices) {
        return false;
    }
    if (stack_size(p->vertices) == 0) {
        stack_push(p->vertices, v);
        return true;
    }
    uint32_t top = 0;
    stack_peek(p->vertices, &top);
    if (G->matrix[top][v]) {
        p->length += G->matrix[top][v];
        stack_push(p->vertices, v);
        return true;
    }
    return false;
}

// pops vertex from path if possible, decrements path length if possible. Returns whether vertex
// was successfully popped.
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (p->length == 0) {
        return false;
    }
    if (stack_size(p->vertices) == 1) {
        stack_pop(p->vertices, v);
        p->length = 0;
        return true;
    }
    stack_pop(p->vertices, v);
    uint32_t top = 0;
    stack_peek(p->vertices, &top);
    p->length -= G->matrix[top][*v];
    return true;
}

// returns number of vertices on the path.
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

// returns length of the path.
uint32_t path_length(Path *p) {
    return p->length;
}

// copies src path to dst path.
void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
    return;
}

// prints path to outfile using cities.
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %" PRIu32 "\n", p->length);
    stack_print(p->vertices, outfile, cities);
}
