#include <stdio.h>

/**
 * Function Name: binaryInsertionSort
 * Description: Sorts an array using the Binary Insertion Sort algorithm.
 *
 * Parameters:
 *   arr (int[]) - The array to be sorted.
 *   n (int) - The number of elements in the array.
 */
void binaryInsertionSort(int arr[], int n) {
    int i, j, key, low, high, mid;

    for (i = 1; i < n; i++) {
        key = arr[i];
        low = 0;
        high = i - 1;

        // Binary search to find the position to insert the current element.
        while (low <= high) {
            mid = (low + high) / 2;
            if (key < arr[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        // Shift elements to make space for the key.
        for (j = i - 1; j >= low; j--) {
            arr[j + 1] = arr[j];
        }

        arr[low] = key;
    }
}

/**
 * Function Name: printArray
 * Description: Prints the elements of an array.
 *
 * Parameters:
 *   arr (int[]) - The array to be printed.
 *   n (int) - The number of elements in the array.
 */
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];
    FILE *file = fopen(inputFile, "rb"); // Use "rb" for binary mode

    if (file == NULL) {
        printf("Couldn't open input file %s\n", inputFile);
        return 1;
    }

    int arr[100]; // Assuming a maximum of 100 integers in the input file
    int n = 0;
    int num;

    while (fread(&num, sizeof(int), 1, file) == 1) {
        arr[n] = num;
        n++;
    }

    fclose(file);

    printf("Unsorted array: \n");
    printArray(arr, n);

    binaryInsertionSort(arr, n);

    printf("Sorted array: \n");
    printArray(arr, n);

    return 0;
}