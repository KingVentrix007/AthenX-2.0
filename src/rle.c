#include "graphics.h"
#include "string.h"
#include "rle.h"
#include "logos.h"
// Function to perform RLE encoding on a 2D input array and return the encoded array size

int rle_encode(int** input_array, int rows, int cols, int* output_array) {
    int output_index = 0;
    int current_element = input_array[0][0];
    int count = 1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (input_array[i][j] == current_element && count < 255) {
                count++;
            } else {
                output_array[output_index++] = count;
                output_array[output_index++] = current_element;
                current_element = input_array[i][j];
                count = 1;
            }
        }
    }

    // Add the last run of elements to the output array
    output_array[output_index++] = count;
    output_array[output_index++] = current_element;

    return output_index;
}

// Function to perform RLE decoding on an input array and return the decoded array size
int rle_decode(int* input_array, int input_size, int** output_array, int rows, int cols) {
    int output_index = 0;

    for (int i = 0; i < input_size; i += 2) {
        int count = input_array[i];
        int element = input_array[i + 1];

        for (int j = 0; j < count; j++) {
            output_array[output_index / cols][output_index % cols] = element;
            output_index++;
        }
    }

    return output_index;
}

int main() {
    int rows = 320;
    int cols = 200;

    // Create a sample 2D input array (matrix)
    int** input_array = logo_img;
    // for (int i = 0; i < rows; i++) {
    //     input_array[i] = (int*)malloc(cols * sizeof(int));
    // }

    // Initialize the input array with some values (for demonstration)
    // int counter = 1;
    // for (int i = 0; i < rows; i++) {
    //     for (int j = 0; j < cols; j++) {
    //         input_array[i][j] = counter++;
    //     }
    // }

    // Calculate the maximum possible size for the encoded array
    int max_encoded_size = rows * cols * 2; // Every element is encoded with a count and a value

    // Allocate memory for the encoded array
    int* encoded_array = (int*)malloc(max_encoded_size * sizeof(int));

    // Encode the input array
    int encoded_size = rle_encode(input_array, rows, cols, encoded_array);

    // Print the encoded array
    printf("Encoded array(%d): ",encoded_size);
    for (int i = 0; i < encoded_size; i++) {
        printf("%d ", encoded_array[i]);
    }
    printf("\n");

    // Allocate memory for the decoded array
    int** decoded_array = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        decoded_array[i] = (int*)malloc(cols * sizeof(int));
    }

    // Decode the encoded array
    int decoded_size = rle_decode(encoded_array, encoded_size, decoded_array, rows, cols);

    // Print the decoded array
    printf("Decoded array(%d):\n",decoded_size);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", decoded_array[i][j]);
        }
        printf("\n");
    }

    // Free the allocated memory (using kfree based on your OS convention)
    for (int i = 0; i < rows; i++) {
        kfree(input_array[i]);
        kfree(decoded_array[i]);
    }
    kfree(input_array);
    kfree(decoded_array);
    kfree(encoded_array);

    return 0;
}