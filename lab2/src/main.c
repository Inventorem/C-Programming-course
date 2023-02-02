#include <stdio.h>
#include <string.h>

//Swaps data at two pointers
void Swap(void *a, void *b, int size) {
    for (int i = 0; i < size; ++i) {
        char temp = *((char *) (b) + i);
        *((char *) (b) + i) = *((char *) (a) + i);
        *((char *) (a) + i) = temp;
    }
}

//Checks, if we can create next row
int Check(const char *row, int len) {
    int temp_string[len];
    int amount[256] = {0};
    for (int i = 0; i < len; ++i) {
        temp_string[i] = row[i] - '0';
        amount[temp_string[i]]++;
        if (row[i] < '0' || row[i] > '9' || amount[temp_string[i]] > 1) {
            return 1;
        }
    }
    return 0;
}

//Creates next row, using Nārāyaṇa's algorithm
int CreateNextRow(char *row, int length) {
    int j = 0, l = 0, flag = 0;
    for (int i = length - 2; i > -1; --i) {
        if (row[i] < row[i + 1]) {
            j = i;
            flag = 1;
            break;
        }
    }
    if (!flag)
        return 1;
    for (int i = length - 1; i > j; i--) {
        if (row[i] > row[j]) {
            l = i;
            break;
        }
    }
    Swap(&row[j], &row[l], sizeof(char));
    for (int i = j + 1; i < (length + j + 1) / 2; ++i) {
        Swap(&row[i], &row[length - i], sizeof(char));
    }
    return 0;
}

//Prints array
void Output(char *row, int length) {
    for (int i = 0; i < length; ++i)
        printf("%c", row[i]);
    printf("\n");
}

int main(void) {
    int length = 0, new_line_count = 0, n;
    char buffer[256];
    for (int i = 0; i < 256; ++i) {
        if (new_line_count == 0) {
            char symbol;
            if (!scanf("%c", &symbol))
                return 0;
            if (symbol != '\n')
                buffer[i] = symbol;
            else {
                new_line_count = 1;
                length = i;
            }
        }
        if (new_line_count == 1) {
            if (!scanf("%d", &n))
                return 0;
            break;
        }
    }
    char row[length];
    memcpy(&row[0], &buffer[0], sizeof(char) * length);
    if (Check(row, length)) {
        printf("bad input");
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        if (CreateNextRow(row, length))
            return 0;
        Output(row, length);
    }
    return 0;
}
