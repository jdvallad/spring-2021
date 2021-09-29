#include <inttypes.h>
#include <stdbool.h>

// All code in this file given in Assignment 4 PDF.
typedef struct Graph Graph;

Graph *graph_create(uint32_t vertices, bool undirected);

void graph_delete(Graph **G);

uint32_t graph_vertices(Graph *G);

bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k);

bool graph_has_edge(Graph *G, uint32_t i, uint32_t j);

uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j);

bool graph_visited(Graph *G, uint32_t v);

bool all_visited(Graph *G);

void graph_mark_visited(Graph *G, uint32_t v);

void graph_mark_unvisited(Graph *G, uint32_t v);

void graph_print(Graph *G);
