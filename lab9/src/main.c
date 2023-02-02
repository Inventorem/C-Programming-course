#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_ARRAY_SIZE 5000
#define NO_PATH UINT_MAX
#define OVERFLOW (NO_PATH - 1)

// Check initial values
void CheckInput(short number, int amount) {
    if (number < 0 || number > MAX_ARRAY_SIZE) {
        printf("bad number of vertices");
        exit(0);
    }
    if (amount < 0 || amount > number * (number + 1) / 2) {
        printf("bad number of edges");
        exit(0);
    }
}

// Check start and finish vertices
void CheckSF(short start, short finish, short n) {
    if (start > n || start < 1) {
        printf("bad vertex");
        exit(0);
    }
    if (finish > n || finish < 1) {
        printf("bad vertex");
        exit(0);
    }
}

// Fill all arrays before running Dijkstra algo
void FillArrays(short number,
                unsigned int ShortWay[MAX_ARRAY_SIZE],
                short PathParent[MAX_ARRAY_SIZE],
                char Visited[MAX_ARRAY_SIZE],
                short start) {
    for (int i = 0; i < number; ++i) {
        ShortWay[i] = NO_PATH;
    }
    ShortWay[start] = 0;
    for (int i = 0; i < number; ++i) {
        PathParent[i] = -1;
    }
    PathParent[start] = start;
    for (int i = 0; i < number; ++i) {
        Visited[i] = 0;
    }
}

// Main algo
void Dijkstra(short number,
              const unsigned int* Matrix,
              unsigned int ShortWays[MAX_ARRAY_SIZE],
              short PathParent[MAX_ARRAY_SIZE],
              char Visited[MAX_ARRAY_SIZE]) {
    short minindex;
    do {
        minindex = SHRT_MAX;
        unsigned int min = NO_PATH;
        for (short i = 0; i < number; i++) {
            if ((Visited[i] == 0) && (ShortWays[i] < min)) {
                min = ShortWays[i];
                minindex = i;
            }
        }
        if (minindex != SHRT_MAX) {
            for (int i = 0; i < number; i++) {
                if (!Visited[i] && Matrix[minindex * number + i]) {
                    long long tempLength =
                            (long long)Matrix[minindex * number + i] + ShortWays[minindex];
                    if (tempLength <= ShortWays[i]) {
                        if (tempLength > INT_MAX) {
                            ShortWays[i] = OVERFLOW;
                        } else {
                            ShortWays[i] = tempLength;
                        }

                        PathParent[i] = minindex;
                    }
                }
            }
            Visited[minindex] = 1;
        } else
            break;
    } while (minindex < SHRT_MAX);
}

int main() {
    short n;
    short start, finish;
    int amount_edges;

    // Scan initial values
    if (scanf("%hd %hd %hd %d", &n, &start, &finish, &amount_edges) < 4) {
        printf("bad number of lines");
        return 0;
    }

    // Check initial values
    CheckInput(n, amount_edges);
    CheckSF(start, finish, n);

    start--, finish--;

    /*      Declaration of all arrays
            Matrix - main 2-d edge-matrix based on linear array
            ShortWay - values of the shortest way to the vertex with current index
            PathParent - previous vertex in the shortest way to current vertex
            Visited - indicates, have we visited this vertex or not
     */
    unsigned int* Matrix = calloc(n * n, sizeof(unsigned int));
    unsigned int ShortWay[MAX_ARRAY_SIZE];
    short PathParent[MAX_ARRAY_SIZE];
    char Visited[MAX_ARRAY_SIZE];

    // Fill arrays with inital values before Dijkstra
    FillArrays(n, ShortWay, PathParent, Visited, start);

    // Edges input
    for (int i = 0; i < amount_edges; ++i) {
        unsigned short from, to;
        long long length;
        if (scanf("%hu %hu %lld", &from, &to, &length) < 3) {
            printf("bad number of lines");
            free(Matrix);
            return 0;
        }
        if (from > n || from < 1) {
            printf("bad vertex");

            free(Matrix);
            return 0;
        }
        if (to > n || to < 1) {
            printf("bad vertex");

            free(Matrix);
            return 0;
        }

        if (length < 0 || length > INT_MAX) {
            printf("bad length");
            free(Matrix);
            return 0;
        }
        from--, to--;
        Matrix[from * n + to] = (unsigned int)length;
        Matrix[to * n + from] = (unsigned int)length;
    }

    // Running main algo
    Dijkstra(n, Matrix, ShortWay, PathParent, Visited);

    // Output of the shortest ways from start vertex
    for (int i = 0; i < n; ++i) {
        if (ShortWay[i] == NO_PATH)
            printf("oo ");
        else if (ShortWay[i] == OVERFLOW)
            printf("INT_MAX+ ");
        else
            printf("%u ", ShortWay[i]);
    }
    printf("\n");

    // Counting edges to finish vertex
    int number_of_edges = 0;
    for (int i = 0; i < n; i++) {
        if (ShortWay[i] != NO_PATH && Matrix[finish * n + i])
            number_of_edges++;
    }

    // Output of the path to finish vertex
    short current_parent = PathParent[finish];
    if (ShortWay[finish] == NO_PATH)
        printf("no path");
    else if (ShortWay[finish] == OVERFLOW && number_of_edges > 1)
        printf("overflow");
    else {
        printf("%hi ", (short)(finish + 1));
        while (current_parent != start) {
            printf("%hi ", (short)(current_parent + 1));
            current_parent = PathParent[current_parent];
        }
        printf("%hi ", (short)(start + 1));
    }

    free(Matrix);
    return EXIT_SUCCESS;
}
