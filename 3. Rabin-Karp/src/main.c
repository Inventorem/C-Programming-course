#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#define max 16

int Pow3(int n) {
    static int Index = 0;
    static int Values[max];
    if (Index == 0) {
        int value = 1;
        for (int i = 0; i < max; ++i) {
            Values[i] = value;
            value *= 3;
        }
        Index = -1;
    }
    return Values[n];
}

int HashFunction(const int* String, int Length) {
    int Hash = 0;
    for (int i = 0; i < Length; i++) {
        Hash += (String[i] % 3) * Pow3(i);
    }
    return Hash;
}

int Shift(int* String, int n, int Hash) {
    int NewSymbol = getc(stdin);
    if (NewSymbol == 208)
        NewSymbol = getc(stdin);
    if (NewSymbol == EOF)
        return EOF;
    Hash -= ((int)String[0] % 3);
    Hash /= 3;
    Hash += (NewSymbol % 3) * Pow3(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        String[i] = String[i + 1];
    }
    String[n - 1] = NewSymbol;
    return Hash;
}

int PrintSimilarities(const int* Sample,
                      const int* Buffer,
                      int Index,
                      int Length) {
    for (int i = 0; i < Length; ++i) {
        printf("%d ", Index + i);
        if (Sample[i] != Buffer[i])
            return 1;
    }
    return 0;
}

int FindSample(int* Sample, int Length) {
    int Hash = HashFunction(Sample, Length);
    int* Buffer = malloc((Length + 1) * sizeof(int));
    for (int i = 0; i < Length; ++i) {
        int NewSymbol = getc(stdin);
        if (NewSymbol == 208 || (NewSymbol == '\n' && i == 0))
            NewSymbol = getc(stdin);
        if (NewSymbol == EOF) {
            free(Buffer);
            return 1;
        } else {
            Buffer[i] = NewSymbol;
        }
    }
    int Index = 1;
    int NewHash = HashFunction(Buffer, Length);
    while (NewHash != EOF) {
        if (NewHash == Hash)
            PrintSimilarities(Sample, Buffer, Index, Length);
        NewHash = Shift(Buffer, Length, NewHash);
        if (NewHash == EOF) {
            free(Buffer);
            return 0;
        }
        Index++;
    }
    free(Buffer);
    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int* Sample = malloc(sizeof(int) * (max + 1));
    int Length = 0;
    for (int i = 0; i < max; ++i) {
        int NewSymbol = getc(stdin);
        if (NewSymbol == 208)
            NewSymbol = getc(stdin);
        if (NewSymbol == '\n') {
            break;
        } else
            Sample[i] = (int)NewSymbol;
        Length++;
    }
    printf("\n%d ", HashFunction(Sample, Length));
    FindSample(Sample, Length);
    free(Sample);
    return 0;
}
