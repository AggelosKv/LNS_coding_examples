#include <iostream>
#include <cstdlib>
#include <cmath>  
#include <random>

void add_bin_vectors(int size1, int size2, int* binaryNum1, int* binaryNum2, int* size3, int* binaryNum3) ;
int decToBinary(int n, int* binaryNum);
void binary2log_mul(int size1, int size2, int* binaryNum1,  int* binaryNum2 ,int* size3, int* binaryNum3);
void binary2decimal(int size, int* binaryNum, int* integer);
void shiftRight(int* arr, int size, int k);
void shiftLeft(int* arr, int size, int k);
void printArray(int* arr, int size);
int changeFirstOneToZero(int* binaryNum, int size);
void copyArray(int* source, int size, int* destination);
void clearArray(int* arr, int size);

// Function to clear an array (set all elements to zero)
void clearArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0; // Set each element to zero
    }
}

// Function to copy elements from one array to another
void copyArray(int* source, int size, int* destination) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i]; // Copy each element from source to destination
    }
}

// Function to change the first 1 from the end of the binaryNum array to 0
int changeFirstOneToZero(int* binaryNum, int size) {
    // Traverse the array from the end to the beginning
    for (int i = size - 1; i >= 0; i--) {
        if (binaryNum[i] == 1) { // Find the first occurrence of 1
            binaryNum[i] = 0;    // Change it to 0
            return i;            // Return the index of the changed position
        }
    }
    return -1; // Return -1 if no 1 is found
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

// function to convert decimal to binary
int decToBinary(int n, int* binaryNum) {
    int i = 0;
    while (n > 0) {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    // Return the size (number of bits)
    return i;
}

// Funtion to convert binary to log and mul using iterations
void binary2log_mul(int size1, int size2, int* binaryNum1,  int* binaryNum2 ,int* size3, int* binaryNum3, int times) {
    int temp_size;
    int size_array1;
    int size_array2;
    temp_size = size1 + size2;
    *size3= temp_size;
    int binarytempNum1[64] = {0}; // Initialize array with zeros 
    int binarytempNum2[64] = {0}; // Initialize array with zeros 
    int binarytempNum3[64] = {0}; // Initialize array with zeros 

    for (int i = 0; i<times; i++){
        size_array1 = changeFirstOneToZero(binaryNum1, size1);      // calculate N-2^k1 
        size_array2 = changeFirstOneToZero(binaryNum2, size2);      // calculate N-2^k2

        if( (size_array1 == -1) | ( size_array2 == -1) ){
            std::cout << " The aproximation fount the value in "<< i <<" iterations!" << "\n";
            break;
        }

        temp_size = size_array1 + size_array2 +2 ;
        copyArray(binaryNum1,temp_size, binarytempNum1);
        copyArray(binaryNum2,temp_size, binarytempNum2);

        shiftLeft(binarytempNum1,temp_size,size_array2);   // calculate (N-2^k1)2^k2
        shiftLeft(binarytempNum2,temp_size,size_array1);   // calculate (N-2^k2)2^k1

        clearArray(binarytempNum3,*size3);
        binarytempNum3[temp_size-2] = 1;   // 2^(k1+k2)
        
        add_bin_vectors(*size3, temp_size, binaryNum3, binarytempNum3, size3, binaryNum3 );     // add to the aproximation
        add_bin_vectors(*size3, temp_size, binaryNum3, binarytempNum2, size3, binaryNum3 );     // add to the aproximation
        add_bin_vectors(*size3, temp_size, binaryNum3, binarytempNum1, size3, binaryNum3 );     // add to the aproximation

    }
}

// Binary vector adder
void add_bin_vectors(int size1, int size2, int* binaryNum1, int* binaryNum2, int* size3, int* binaryNum3) {
    // Initialize carry
    int carry = 0;
    int maxSize = *size3 ; // Determine the maximum size

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

// Function to convert binary array to decimal
void binary2decimal(int size, int* binaryNum, int* integer) {
    *integer = 0; // Initialize integer to 0
int temp = 1;
    // Iterate over the binary array
    for (int i = 0; i < size; i++) {
        *integer = *integer + binaryNum[i]*temp; 
        temp = temp*2;
    }
}


// Function to generate a random floating-point number between 0 and 20000
double generateRandomFloat(double maxValue = 20000) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<double> dist( 1, maxValue);
    
    return dist(gen);
}

// Driver program to test the above function
int main(int argc, char* argv[]) {
    int n1;
    int n2;
    int times = 7;
    int size1;
    int size2;
    int size3;
    int integer;
    float correct;
    int fin = 1000;
    int binaryNum1[64] = {0}; // Initialize array with zeros
    int binaryNum2[64] = {0}; // Initialize array with zeros 
    int binaryNum3[64] = {0}; // sum of the n1+n2 binary values in logarithmic domain

    for (int i = 0; i< fin; i++){
        n1 = (int)(generateRandomFloat());
        n2 = (int)(generateRandomFloat());


        // Array to store binary number (32 bits should be enough for typical integers)
        for (int j = 0; j < 64; j++){
            binaryNum1[j] = 0; // Initialize array with zeros
            binaryNum2[j] = 0; // Initialize array with zeros 
            binaryNum3[j] = 0; // sum of the n1+n2 binary values in logarithmic domain
        }

        // Call the function with pointer to binaryNum
        size1 = decToBinary(n1, &binaryNum1[0]);
        size2 = decToBinary(n2, &binaryNum2[0]);
        size3 = 0;

        // Output the binary number1 in reverse order
        std::cout << "Binary of num1 " << n1 << " is: ";
        printArray(binaryNum1,size1);
        std::cout << "Binary of num2 " << n2 << " is: ";
        printArray(binaryNum2,size2);

        binary2log_mul(size1, size2, &binaryNum1[0], &binaryNum2[0], &size3, &binaryNum3[0], times);

        std::cout << "Binary of num3 is: ";
        printArray(binaryNum3,size3);
        std::cout << "\n";



        binary2decimal(size3,&binaryNum3[0],&integer);


        std::cout << "The mul of " << n1 << " and "<< n2 << " is: " <<  integer  << "\n";
        correct = n1 * n2;
        std::cout << "The correct result is: " << correct << "\n";
        correct = correct - integer;    // calculate the error (we could use double for better accuracy)
        std::cout << "The error between them is: " << correct << "\n";
        std::cout << std::endl;
        if (correct != 0){
            break;
        }
    }

    return 0;
}