#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 5000
#define MAX_LENGTH INT_MAX

//Struct for edges
typedef struct edge {
    short from;
    short to;
    int length;
} Edge;

//Free all malloc-arrays
void FreeArrays(Edge* edges, short* set, short* weight, short* frame) {
    free(edges);
    free(set);
    free(weight);
    free(frame);
}

//Initial filling of arrays
void Initialization(short* set, short* weight, short* frame, short n) {
    for (short i = 0; i < n; ++i) {
        set[i] = i;
        weight[i] = 1;
        frame[2 * i] = -1;
        frame[2 * i + 1] = -1;
    }
}

//Return first vertex in a set
short getSetLeader(short* set, short v) {
    if (v == set[v]) {
        return v;
    }
    return set[v] = getSetLeader(set, set[v]);
}

// Swap values in a and b, a and b -  short values
void SwapShort(short* a, short* b) {
    short temp = *a;
    *a = *b;
    *b = temp;
}

//Unite two sets based on path-compression and tree-weight optimizations
void Unite(short* set, short* weight, short a, short b) {
    a = getSetLeader(set, a);
    b = getSetLeader(set, b);
    if (weight[a] > weight[b]) {
        SwapShort(&weight[a], &weight[b]);
    }
    weight[b] += weight[a];
    if (a != b) {
        set[a] = b;
    }
}

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

// Swap values in A and B, A and B -  edges
void SwapEdges(Edge* A, Edge* B) {
    Edge temp = *A;
    *A = *B;
    *B = temp;
}

//Classical qsort from lab 3-1, modified for edge-struct
int QuickSort(Edge* Array, int Start, int End) {
    if (End <= Start) {
        return 0;
    }
    int i = Start;
    int j = End;
    int pivot = Array[(i - (i - j) / 2)].length;
    do {
        while (Array[i].length < pivot)
            i++;
        while (Array[j].length > pivot)
            j--;
        if (i <= j) {
            SwapEdges(&Array[i], &Array[j]);
            i++, j--;
        }
    } while (i <= j);
    QuickSort(Array, Start, j);
    QuickSort(Array, i, End);
    return 0;
}

//Main algo
bool Kruskal(Edge* edges,
             short* set,
             short* weight,
             short* frame,
             short n,
             int m) {
    Initialization(set, weight, frame, n);
    short EdgesCount = 0;
    for (int i = 0; i < m; i++) {
        //Add edge, if two vertices are from different sets
        if (getSetLeader(set, edges[i].from) != getSetLeader(set, edges[i].to)) {
            Unite(set, weight, edges[i].from, edges[i].to);
            frame[2 * EdgesCount] = edges[i].from;
            frame[2 * EdgesCount + 1] = edges[i].to;
            EdgesCount++;
        }
    }
    EdgesCount++;
    if (EdgesCount != n) {
        return false;
    }
    return true;
}

int main() {
    short n = 0;
    int m = 0;
    if (scanf("%hi %d", &n, &m) < 2) {
        printf("bad number of lines");
        return 0;
    }
    CheckInput(n, m);

    /*  set - number of the set for the following vertex
     *  weight - weight of the subtree for the following vertex
     *  frame - array with edges for minimal frame, sorted increasingly
     */
    Edge* edges = malloc(sizeof(Edge) * m);
    short* set = malloc(sizeof(short) * n);
    short* weight = malloc(sizeof(short) * n);
    short* frame = malloc(sizeof(short) * 2 * n);

    //Edges input
    for (int i = 0; i < m; i++) {
        short from;
        short to;
        long long length;

        if (scanf("%hd %hd %lld", &from, &to, &length) < 3) {
            printf("bad number of lines");
            FreeArrays(edges, set, weight, frame);
            return 0;
        }
        CheckEdge(from, to, length, n);

        from--;
        to--;
        edges[i] = (Edge){from, to, (int)length};
    }

    //Qsort of edges
    QuickSort(edges, 0, m - 1);

    //Frame check and output of the frame
    if (!Kruskal(edges, set, weight, frame, n, m)) {
        printf("no spanning tree");
        FreeArrays(edges, set, weight, frame);
        return 0;
    } else {
        int k = 0;
        while (frame[2 * k] != -1) {
            printf("%hi %hi\n", (short)(frame[2 * k] + 1),
                   (short)(frame[2 * k + 1] + 1));
            k++;
        }
    }

    FreeArrays(edges, set, weight, frame);
}
