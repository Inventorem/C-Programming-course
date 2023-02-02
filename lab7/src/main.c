#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Write "1" in a bit for from-to edge
void writeEdge(char matrix[], unsigned short from, unsigned to, unsigned n) {
    int byte = (from * n + to) / 8;
    char position = (from * n + to) % 8;
    matrix[byte] |= (1 << position);
}

// Check for Edge between "from" and "to" vertices
char isEdge(const char matrix[], unsigned short from, unsigned to, unsigned n) {
    int byte = (int)((from * n + to) / 8);
    char position = (from * n + to) % 8;
    return matrix[byte] & (1 << position);
}

// Common DFS+writing answer array
bool DFS(char matrix[],
         char* color,
         unsigned short* answerIndex,
         unsigned short* answer,
         unsigned short n,
         unsigned short index) {
    // Coloring current vertex as visited
    color[index] = 1;
    // Checking for edges with current vertex
    for (unsigned short i = 0; i < n; ++i) {
        if (isEdge(matrix, index, i, n)) {
            if (color[i] == 1) {
                printf("impossible to sort");
                return false;
            } else if (color[i] == 0) {
                if (!DFS(matrix, color, answerIndex, answer, n, i))
                    return false;
            }
        }
    }
    // Writing current vertex in answer
    color[index] = 2;
    answer[*answerIndex] = index;
    (*answerIndex)++;
    return true;
}

// Topological sort based on DFS
bool TopologicalSort(char matrix[],
                     char* color,
                     unsigned short* answer,
                     unsigned short n) {
    unsigned short answerIndex = 0;
    for (unsigned short i = 0; i < n; i++) {
        if (!color[i])
            if (!DFS(matrix, color, &answerIndex, answer, n, i))
                return false;
    }
    return true;
}

// Fill arrays with 0
void ZeroFill(char matrix[],
              char* color,
              unsigned short* answer,
              unsigned short n) {
    for (int i = 0; i < 500000; i++) {
        matrix[i] = 0;
    }
    for (unsigned i = 0; i < n; i++) {
        color[i] = 0;
        answer[i] = 0;
    }
}

// Free arrays for color and answer
void FreeArrays(char* color, unsigned short* answer) {
    free(color);
    free(answer);
}

int main(void) {
    char matrix[500000];

    short n;
    if (!scanf("%hi", &n)) {
        printf("bad number of lines");
        return 0;
    }
    if (n < 0 || n > 2000) {
        printf("bad number of vertices");
        return 0;
    }
    int m = -1;
    if (!scanf("%d", &m)) {
        printf("bad number of lines");
        return 0;
    }
    if (m == -1) {
        printf("bad number of lines");
        return 0;
    }
    if (m < 0 || m > (n * (n - 1))) {
        printf("bad number of edges");
        return 0;
    }

    // Create and fill arrays
    char* color = (char*)malloc(sizeof(char) * n);
    unsigned short* answer = (unsigned short*)malloc(sizeof(unsigned short) * n);
    ZeroFill(matrix, color, answer, n);

    // Edges input
    for (int i = 0; i < m; ++i) {
        unsigned short from, to;
        if (scanf("%hu %hu", &from, &to) < 2) {
            printf("bad number of lines");
            FreeArrays(color, answer);
            return 0;
        }
        if (from > n || from < 1) {
            printf("bad vertex");

            FreeArrays(color, answer);
            return 0;
        }
        if (to > n || to < 1) {
            printf("bad vertex");

            FreeArrays(color, answer);
            return 0;
        }
        to--, from--;
        writeEdge(matrix, from, to, n);
    }

    // TopSort
    if (!TopologicalSort(matrix, color, answer, n)) {
        FreeArrays(color, answer);
        return 0;
    }

    // Output
    for (int i = 0; i < n; ++i) {
        printf("%d ", answer[(n - 1) - i] + 1);
    }
    FreeArrays(color, answer);
    return EXIT_SUCCESS;
}
