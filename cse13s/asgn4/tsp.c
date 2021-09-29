#include "graph.c"
#include "path.c"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PARAMS  "ghvui:o:"
#define MAX_STR 100

// Flags to keep track of command line parameters
// recursive_calls is used to keep track of how many times dfs is called
bool undirected = false;
bool graph = false;
bool help = false;
bool verbose = false;
FILE *infile = NULL;
FILE *outfile = NULL;
int recursive_calls = 0;

// Parses the command line arguments, setting flags to the appropriate state.
void parse_arguments(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, PARAMS)) != -1) {
        switch (opt) {
        case 'h': help = true; return;
        case 'v': verbose = true; break;
        case 'u': undirected = true; break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'g': graph = true; break;
        default: break;
        }
    }
}

// Prints the help message if the -h option is present
void help_message(void) {
    char cwd[100];
    getcwd(cwd, 100);
    printf("SYNOPSIS\n");
    printf("  Traveling Salesman Problem using DFS.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  %s/tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n", cwd);
    printf("\n");
    printf("OPTIONS\n");
    printf("  -u             Use undirected graph.\n");
    printf("  -v             Enable verbose printing.\n");
    printf("  -h             Program usage and help.\n");
    printf("  -g             Print input graph to stdout and wait for user to continue.\n");
    printf("  -i infile      Input containing graph (default: stdin)\n");
    printf("  -o outfile     Output of computed path (default: stdout)\n");
    return;
}

//stores the next line from file into in, up until max chars. Newlines are removed from in.
char *next_line(char *in, int max, FILE *file) {
    if ((fgets(in, max, file)) == NULL) {
        return NULL;
    }
    int index = 0;
    while (in[index] != '\0') {
        index++;
    }
    in[index - 1] = '\0';
    return in;
}

// after finding shortest path, it is written to specified file here.
void write_output(FILE *file, Path *path, char *cities[]) {
    path_print(path, file, cities);
    fprintf(outfile == NULL ? stdout : outfile, "Total recursive calls: %d\n", recursive_calls);
    return;
}

// closes infile and outfile in necessary
void close_files() {
    if (infile != NULL) {
        fclose(infile);
    }
    if (outfile != NULL) {
        fclose(outfile);
    }
    return;
}

// frees memory for cities
void free_cities(char *cities[], int vertices) {
    for (int i = 0; i < vertices; i++) {
        free(cities[i]);
        cities[i] = NULL;
    }
    return;
}
// Uses the input file to populate cities[] and create and return a graph.
Graph *read_input(char *cities[], FILE *file) {
    int vertices;
    char line[MAX_STR];
    if (next_line(line, MAX_STR, file) == NULL) {
        printf("Error: malformed number of vertices.\n");
        close_files();
        return NULL;
    }
    vertices = atoi(line);
    if (vertices < 2) {
        printf("There's nowhere to go.\n");
        close_files();
        return NULL;
    }
    Graph *G = graph_create(vertices, undirected);
    for (int i = 0; i < vertices; i++) {
        next_line(line, MAX_STR, file);
        cities[i] = (char *) malloc(sizeof(char) * MAX_STR);
        strcpy(cities[i], line);
    }
    while (true) {
        char *res = next_line(line, MAX_STR, file);
        if (res == NULL || *res == '\0') {
            break;
        }
        uint32_t a = 0, b = 0, c = 0;
        char *token = strtok(line, " ");
        if (token == NULL) {
            printf("Malformed edge.\n");
            free_cities(cities, vertices);
            graph_delete(&G);
            close_files();
            return NULL;
        }
        a = atoi(token);
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Malformed edge.\n");
            free_cities(cities, vertices);
            graph_delete(&G);
            close_files();
            return NULL;
        }
        b = atoi(token);
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Malformed edge.\n");
            free_cities(cities, vertices);
            graph_delete(&G);
            close_files();
            return NULL;
        }
        c = atoi(token);
        graph_add_edge(G, a, b, c);
    }
    if (G == NULL) {
        free_cities(cities, vertices);
        graph_delete(&G);
        close_files();
    }
    return G;
}

// This is the actual recursive function that is used to find the shortest path.
// Recursively checks every possible path and keeps track of the shortest one. Traverse forward and
// backward recursively through each vertex of the graph until a hamiltonian path is found, which is
// then checked against the current shortest path.
void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile) {
    recursive_calls++;
    graph_mark_visited(G, v);
    path_push_vertex(curr, v, G);
    uint32_t temp = 0;
    if (path_length(shortest) != 0 && path_length(curr) >= path_length(shortest)) {
        graph_mark_unvisited(G, v);
        path_pop_vertex(curr, &temp, G);
        return;
    }
    if (all_visited(G) && graph_has_edge(G, v, START_VERTEX)) {
        path_push_vertex(curr, START_VERTEX, G);
        if (path_length(shortest) == 0 || path_length(curr) < path_length(shortest)) {
            path_copy(shortest, curr);
        }
        if (verbose) {
            path_print(curr, outfile, cities);
        }
        path_pop_vertex(curr, &temp, G);
    }
    for (uint32_t i = 0; i < graph_vertices(G); i++) {
        if (graph_has_edge(G, v, i) && (!graph_visited(G, i))) {
            dfs(G, i, curr, shortest, cities, outfile);
        }
    }
    graph_mark_unvisited(G, v);
    path_pop_vertex(curr, &temp, G);
    return;
}

// closes all memory on the heap to prevent memory leaks
void memory_mop(char *cities[], Graph **G, Path **shortest, Path **current) {
    free_cities(cities, graph_vertices(*G));
    graph_delete(G);
    path_delete(current);
    path_delete(shortest);
    close_files();
    return;
}
// main function that is run with program.
// Reads in user input, and then uses that to construct graph.
// Shortest path is then found and printed to the screen.
// Finally, heap memory is then freed and program returns.
int main(int argc, char **argv) {
    char *cities[VERTICES];
    parse_arguments(argc, argv);
    if (help) {
        help_message();
        if (infile != NULL) {
            fclose(infile);
        }
        if (outfile != NULL) {
            fclose(outfile);
        }
        return 0;
    }
    Graph *G = read_input(cities, infile == NULL ? stdin : infile);
    if (G == NULL) {
        return 0;
    }
    if (graph) {
        graph_print_cities(G, cities);
        printf("Press any button to continue:");
        getchar();
    }
    Path *current_path = path_create();
    Path *shortest_path = path_create();
    recursive_calls = 0;
    dfs(G, START_VERTEX, current_path, shortest_path, cities, outfile == NULL ? stdout : outfile);
    write_output(outfile == NULL ? stdout : outfile, shortest_path, cities);
    memory_mop(cities, &G, &shortest_path, &current_path);
    return 0;
}
