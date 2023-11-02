#ifndef RLE_H
#define RLE_H
int rle_encode(int** input_array, int rows, int cols, int* output_array);// Function to perform RLE encoding on a 2D input array and return the encoded array size


int rle_decode(int* input_array, int input_size, int** output_array, int rows, int cols);// Function to perform RLE decoding on an input array and return the decoded array size
int main();
#endif