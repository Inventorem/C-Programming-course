#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5000
#define MAX_LENGTH INT_MAX

typedef struct FrameVerticesPair {
    short from;
    short to;
} Pair;

void CheckInput(int n, int m) {
    if (n < 0 || n > MAX_SIZE) {
        printf("bad number of vertices");
        exit(0);
    }

    if (m < 0 || m > (int)(n * (n - 1) / 2)) {
        printf("bad number of edges");
        exit(0);
    }

    if (n == 0) {
        printf("no spanning tree");
        exit(0);
    }
}

void CheckEdge(short from, short to, long long int length, int n) {
    if (from < 1 || from > n) {
        printf("bad vertex");
        exit(0);
    }
    if (to < 1 || to > n) {
        printf("bad vertex");
        exit(0);
    }
    if (length < 0 || length > MAX_LENGTH) {
        printf("bad length");
        exit(0);
    }
}

void WriteEdge(int* matrix, short from, short to, long long int length, int n) {
    matrix[from * n + to] = (int)length;
    matrix[to * n + from] = (int)length;
}

void Initialization(const int* matrix,
                    short n,
                    char* used,
                    int* minimalEdge,
                    short* fromVertex) {
    used[1] = true;
    for (short i = 0; i < n + 1; i++) {
        if (matrix[i + n]) {
            minimalEdge[i] = matrix[i + n];
            fromVertex[i] = 1;
        } else {
            minimalEdge[i] = 0;
            fromVertex[i] = 0;
        }
    }
}

bool Prim(const int* matrix, short n, char* used, Pair* frame) {
    short ind = 0;
    int* minimalEdge = malloc(sizeof(int) * (n + 1));
    short* fromVertex = malloc(sizeof(short) * (n + 1));

    Initialization(matrix, n, used, minimalEdge, fromVertex);

    for (short i = 1; i < n; i++) {
        short next_vertex = 0;
        for (short j = 1; j < n + 1; j++) {
            if (!used[j] &&
                (!next_vertex || !minimalEdge[next_vertex] ||
                 (minimalEdge[j] && minimalEdge[j] < minimalEdge[next_vertex])))
                next_vertex = j;
        }

        if (!minimalEdge[next_vertex]) {
            printf("no spanning tree");
            free(minimalEdge);
            free(fromVertex);
            return false;
        }

        frame[ind++] = (Pair){next_vertex, fromVertex[next_vertex]};
        used[next_vertex] = true;
        for (short j = 1; j < n + 1; j++) {
            if (matrix[next_vertex * n + j] &&
                (!minimalEdge[j] || matrix[next_vertex * n + j] < minimalEdge[j])) {
                minimalEdge[j] = matrix[next_vertex * n + j];
                fromVertex[j] = next_vertex;
            }
        }
    }

    free(minimalEdge);
    free(fromVertex);
    return true;
}

void FreeArrays(int* matrix, char* used, Pair* frame) {
    free(matrix);
    free(used);
    free(frame);
}

int main() {
    short n = 0;
    int m = 0;
    if (scanf("%hi %d", &n, &m) < 2) {
        printf("bad number of lines");
        return 0;
    }
    CheckInput(n, m);

    int* matrix = calloc((n + 1) * (n + 1), sizeof(int));
    char* used = calloc(n + 1, sizeof(short));
    Pair* frame = malloc(sizeof(Pair) * n);

    // Edges input
    for (int i = 0; i < m; i++) {
        short from;
        short to;
        long long length;
        if (scanf("%hd %hd %lld", &from, &to, &length) < 3) {
            printf("bad number of lines");
            FreeArrays(matrix, used, frame);
            return 0;
        }

        CheckEdge(from, to, length, n);
        WriteEdge(matrix, from, to, length, n);
    }

    // Frame check
    if (Prim(matrix, n, used, frame)) {
        // Output of the frame
        for (int i = 0; i < n - 1; i++) {
            printf("%d %d\n", (frame[i].from), (frame[i].to));
        }
    }

    FreeArrays(matrix, used, frame);
}
