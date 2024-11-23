#include <iostream>
#include <algorithm> // For std::max


void add_bin_vectors(int size1, int size2, int* binaryNum1, int* binaryNum2, int* size3, int* binaryNum3) {
    // Initialize carry
    int carry = 0;
    int maxSize = std::max(size1, size2); // Determine the maximum size

    // Iterate over each bit from right to left
    for (int i = 0; i < maxSize; i++) {
        // Get the current bits from each binary number, if available
        int bit1 = (i < size1) ? binaryNum1[i] : 0; // From binaryNum1 (reverse index)
        int bit2 = (i < size2) ? binaryNum2[i] : 0; // From binaryNum2 (reverse index)

        // Calculate the sum of the two bits and the carry
        int sum = bit1 + bit2 + carry;
        
        // Determine the result bit and the new carry
        binaryNum3[i] = sum % 2; // Result bit (0 or 1)
        carry = sum / 2;         // Carry for the next bit
    }

    // If there's a carry left after the last addition, add it to the result
    if (carry > 0) {
        binaryNum3[maxSize] = carry; // Place carry at the next position
        *size3 = maxSize + 1; // New size includes the carry
    } else {
        *size3 = maxSize; // No carry, size remains maxSize
    }
}

// Function to shift the elements of an array to the left by k spaces
void shiftRight(int* arr, int size, int k) {
    if (size <= 0 || k <= 0) return; // No elements to shift

    // Normalize k in case it's larger than the array size
    k = k % size;

    // Create a temporary array to hold the shifted values
    int* temp = new int[size];

    // Shift elements to the left
    for (int i = 0; i < size; i++) {
        temp[i] = arr[(i + k) % size]; // Shift and wrap around
    }

    // Copy back to the original array
    for (int i = 0; i < size; i++) {
        arr[i] = temp[i];
    }

    delete[] temp; // Free the allocated memory
}

// Function to shift the elements of an array to the right by k spaces
void shiftLeft(int* arr, int size, int k) {
    if (size <= 0 || k <= 0) return; // No elements to shift

    // Normalize k in case it's larger than the array size
    k = k % size;

    // Create a temporary array to hold the shifted values
    int* temp = new int[size];

    // Shift elements to the right
    for (int i = 0; i < size; i++) {
        temp[i] = arr[(i - k + size) % size]; // Shift and wrap around
    }

    // Copy back to the original array
    for (int i = 0; i < size; i++) {
        arr[i] = temp[i];
    }

    delete[] temp; // Free the allocated memory
}

// Function to print the array
void printArray(int* arr, int size) {
    for (int j = size - 1; j >= 0; j--) {
        std::cout << arr[j];
    }
    std::cout << std::endl;
}


// Function to convert decimal to binary
int decToBinary(int n, int* binaryNum) {
    int i = 0;
    while (n > 0) {
        // Store remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    // Return the size (number of bits)
    return i;
}


// Driver program to test the above functions
int main(int argc, char* argv[]) {
    int n1 = 234; // First decimal number
    int n2 = 198; // Second decimal number

    // Array to store binary number (64 bits should be enough for typical integers)
    int binaryNum1[64] = {0}; // Initialize array with zeros
    int binaryNum2[64] = {0}; // Initialize array with zeros 
    int binaryNum3[64] = {0}; // Sum of the n1 + n2 binary values

    // Call the function with pointer to binaryNum
    int size1 = decToBinary(n1, &binaryNum1[0]);
    int size2 = decToBinary(n2, &binaryNum2[0]);
    int size3 = size1 + size2 + 3+ 1;

    // Output the binary number1 in reverse order
    std::cout << "Binary of num1 " << n1 << " is: ";
    printArray(binaryNum1,size1);
    std::cout << "Binary of num2 " << n2 << " is: ";
    printArray(binaryNum2,size2);

binaryNum1[size1-1]=0;
binaryNum2[size2-1]=0;


    int temp_size;
    temp_size = size1 + size2;
    shiftLeft(binaryNum1,temp_size,size2-1);
    shiftLeft(binaryNum2,temp_size,size1-1);
    // size3 = temp_size + 1;
    // binaryNum3[size3-2] = 1;

    // Output the binary number1 in reverse order
    std::cout << "NEW Binary of num1 " << n1 << " is: ";
    printArray(binaryNum1,temp_size);
    std::cout << "NEW Binary of num2 " << n2 << " is: ";
    printArray(binaryNum2,temp_size);
    std::cout << "NEW Binary of num3 is:    ";
    printArray(binaryNum3,size3);

    // Call the addition function
    add_bin_vectors(temp_size, temp_size, binaryNum1, binaryNum2, &size3, binaryNum3);

    // Print the result
    std::cout << "Result of binary addition: ";
        printArray(binaryNum3,size3);
    std::cout << "Binary of num1 : " << n1 << " is: ";
    std::cout << std::endl;

    return 0;
}