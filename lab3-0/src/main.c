#include <stdio.h>
#include <stdlib.h>

void Swap(int* A, int* B) {
  int temp = *A;
  *A = *B;
  *B = temp;
}

// This function checks if i, 2i+1 and 2i+2 elements satisfy heap condition and
// checks all attached triples
void CreateHeap(int* Array, int n, int i) {
  int Root = i;
  int FirstChild = 2 * i + 1;
  int SecondChild = 2 * i + 2;
  // If one of the children is bigger, than root, change root
  if (FirstChild < n && Array[FirstChild] > Array[Root])
    Root = FirstChild;
  if (SecondChild < n && Array[SecondChild] > Array[Root])
    Root = SecondChild;
  // if the root has changed, select new root and check all elements involved by
  // this change
  if (Root != i) {
    // swap root and child that bigger than root;
    Swap(&Array[i], &Array[Root]);
    // check attached elements
    CreateHeap(Array, n, Root);
  }
}

void HeapSort(int* NonSortedArray, int n) {
  // Creating initial heap
  for (int i = n / 2 - 1; i > -1; i--) {
    CreateHeap(NonSortedArray, n, i);
  }
  for (int i = n - 1; i > -1; i--) {
    // throw max element  to the end and recreate the heap
    Swap(&NonSortedArray[0], &NonSortedArray[i]);
    CreateHeap(NonSortedArray, i, 0);
  }
}

int main() {
  int n;
  if (scanf("%d", &n) == 0)
    return 0;
  int* arr = malloc(sizeof(int) * n);

  // Input of array elements
  for (int i = 0; i < n; ++i) {
    if (scanf("%d", &arr[i]) == 0)
      return 0;
  }

  HeapSort(arr, n);

  // Output of array elements
  for (int i = 0; i < n; ++i) {
    printf("%d ", arr[i]);
  }
  free(arr);
  return 0;
}
