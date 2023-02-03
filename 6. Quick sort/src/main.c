#include <stdlib.h>
#include <stdio.h>

// Swaps values in A and B
void Swap(int* A, int* B) {
  int temp = *A;
  *A = *B;
  *B = temp;
}

int QuickSort(int* Array, int Start, int End) {
  if (End <= Start) {
    return 0;
  }
  int i = Start;
  int j = End;
  int pivot = Array[(i - (i - j) / 2)];
  do {
    while (Array[i] < pivot)
      i++;
    while (Array[j] > pivot)
      j--;
    if (i <= j) {
      Swap(&Array[i], &Array[j]);
      i++, j--;
    }
  } while (i <= j);
  QuickSort(Array, Start, j);
  QuickSort(Array, i, End);
  return 0;
}

int main() {
  int n;
  if (!(scanf("%d", &n)))
    return 0;
  int* Array = malloc(n * sizeof(int));
  // Array Input
  for (int i = 0; i < n; i++) {
    if (!(scanf("%d", &Array[i])))
      return 0;
  }
  // Sorting
  QuickSort(Array, 0, n - 1);

  // Array output
  for (int i = 0; i < n; printf("%d ", Array[i++]))
    ;

  free(Array);
  return 0;
}
