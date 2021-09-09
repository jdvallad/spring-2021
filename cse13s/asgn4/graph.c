#include "graph.h"

#include "vertices.h"

#include <stdio.h>
#include <stdlib.h>
// struct code given in assignment 4 PDF.
struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

// Graph constructor, allocated proper amount of memory to store a graph
Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) malloc(sizeof(Graph));
    if (G) {
        G->vertices = vertices;
        G->undirected = undirected;
        for (uint32_t i = 0; i < vertices; i++) {
            G->visited[i] = 0;
            for (uint32_t j = 0; j < vertices; j++) {
                G->matrix[i][j] = 0;
            }
        }
    }
    return G;
}

// Graph deleter, frees up memory taken up by G when it is no longer needed
void graph_delete(Graph **G) {
    if (*G) {
        free(*G);
        *G = NULL;
    }
    return;
}

// returns the number of vertices on graph G
uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

// Adds an edge to graph G
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i < G->vertices && j < G->vertices) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

// Checks for existence of edge in graph G
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    return i < G->vertices && j < G->vertices && G->matrix[i][j];
}

// returns edge weight in graph G
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (i >= G->vertices || j >= G->vertices) {
        return 0;
    }
    return G->matrix[i][j];
}

// checks for vertex visit status in graph G
bool graph_visited(Graph *G, uint32_t v) {
    return v < G->vertices && G->visited[v];
}

// checks to see if every vertex has been visited
bool all_visited(Graph *G) {
    for (uint32_t i = 0; i < G->vertices; i++) {
        if (!G->visited[i]) {
            return false;
        }
    }
    return true;
}

// marks a vertex as visited in graph G
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = true;
    }
    return;
}

// marks a vertex as unvisited in graph G
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = false;
    }
    return;
}

// This will produces graphs in the following form:
//
//       0   1   2   3
//    -------=--------
//   0|100  10   0   0
//    |
//   1|  0   0   0   0
//    |
//   2|  0   0   0   0
//    |
//   3|  0   0   0   0
//
//   = instead of - signifies that that vertex has been visited
void graph_print(Graph *G) {
    printf("\n   ");
    for (uint32_t i = 0; i < G->vertices; i++) {
        printf(" %3" PRIu32, i);
    }
    printf("\n   ---");
    for (uint32_t i = 0; i < G->vertices - 1; i++) {
        printf("%s---", G->visited[i] ? "=" : "-");
    }
    printf("%s\n", G->visited[G->vertices - 1] ? "=" : "-");
    for (uint32_t i = 0; i < G->vertices; i++) {
        printf("%3" PRIu32 "|", i);
        for (uint32_t j = 0; j < G->vertices; j++) {
            printf("%3" PRIu32 " ", G->matrix[i][j]);
        }
        printf("\n   %s\n", i == G->vertices - 1 ? "" : "|");
    }
    printf("\n");
    return;
}
void graph_print_cities(Graph *G, char *cities[]) {
    printf("\n                        ");
    for (uint32_t i = 0; i < G->vertices; i++) {
        printf(" %3" PRIu32, i);
    }
    printf("\n                        ---");
    for (uint32_t i = 0; i < G->vertices - 1; i++) {
        printf("%s---", G->visited[i] ? "=" : "-");
    }
    printf("%s\n", G->visited[G->vertices - 1] ? "=" : "-");
    for (uint32_t i = 0; i < G->vertices; i++) {
        printf("%20s %3" PRIu32 "|", cities[i], i);
        for (uint32_t j = 0; j < G->vertices; j++) {
            printf("%3" PRIu32 " ", G->matrix[i][j]);
        }
        printf("\n%s\n", i == G->vertices - 1 ? "" : "                        |");
    }
    printf("\n");
    return;
}
