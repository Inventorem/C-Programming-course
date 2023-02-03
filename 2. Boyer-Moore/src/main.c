#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#define BuffSize 10000


//Calculates shift value for given sample and symbol
int ShiftCalc(const unsigned char *Sample, int length, unsigned char symbol) {
    int Shift = length;
    for (int i = 0; i < length - 1; ++i) {
        if (symbol == Sample[i]) {
            Shift = length - 1 - i;
        }
    }
    return Shift;
}

//Rewrites buffer if index is out of bounds of buffer
int BufferRewrite(unsigned char *Buffer, int *PositionInBuffer, int length) {
    int NewElementsCount = *PositionInBuffer - length + 1;
    if (*PositionInBuffer < BuffSize - 1 + length)
        memcpy(&Buffer[0], &Buffer[BuffSize - NewElementsCount], NewElementsCount * sizeof(unsigned char));
    *PositionInBuffer = length - 1;
    return (int) fread(&Buffer[BuffSize - NewElementsCount], sizeof(unsigned char), NewElementsCount, stdin);
}

//Boyer-Moore algorithm function
int Boyer_Moore(const unsigned char *Sample, int length) {
    unsigned char Buffer[BuffSize];
    int Written = (int) fread(Buffer, sizeof(unsigned char), BuffSize, stdin);
    int PositionInBuffer;
    int Checked = 0;
    for (PositionInBuffer = length - 1; PositionInBuffer < Written && Written > 0;) {
        int IsSimilar = 0;
        //Comparison and shift part
        for (int i = length - 1; i > -1; --i) {
            int ComparisonIndex = PositionInBuffer - length + i + 1;
            printf("%d ", ComparisonIndex + Checked + 1);
            if (Sample[i] == Buffer[ComparisonIndex]) {
                IsSimilar = 1;
                if (!i) {
                    PositionInBuffer += length;
                    break;
                }
            } else if (!IsSimilar) {
                PositionInBuffer += ShiftCalc(Sample, length, Buffer[ComparisonIndex]);
                break;
            } else {
                PositionInBuffer += ShiftCalc(Sample, length, Sample[length - 1]);
                break;
            }
        }
        if (PositionInBuffer >= BuffSize) {
            Written = BufferRewrite(Buffer, &PositionInBuffer, length);
            Checked += BuffSize;
        }
    }
    return 0;
}

int main(void) {
    setlocale(LC_ALL, "Russian");
    unsigned char *Sample = malloc(16 * sizeof(char));
    int length = 0;
    for (int i = 0; i < 17; ++i) {
        unsigned char Symbol;
        if (!scanf("%c", &Symbol)) {
            free(Sample);
            return 0;
        }
        if (Symbol != '\n') {
            Sample[i] = Symbol;
            length++;
        } else
            break;
    }
    Boyer_Moore(Sample, length);
    free(Sample);
    return 0;
}
