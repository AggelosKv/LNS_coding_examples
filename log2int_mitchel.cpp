#include <cstdlib>
#include <iostream>
#include <cmath>
#include <random>
#include <iomanip> // For std::setprecision

// Function to convert a binary number to a double
void binaryToDouble(int* size, int* frac_start, int* binaryNum, double* number) {
    // Validate inputs
    if (*size <= 0 || *frac_start < 0 || *frac_start >= *size) {
        *number = 0; // Invalid input; set to 0
        return;
    }

    // Calculate the integer part
    double integerPart = 0.0;
    double base = 1.0; // This will represent 2^0, 2^1, 2^2, ...
    
    for (int i = *frac_start - 1; i >= 0; i--) {
        integerPart += binaryNum[i] * base; // binaryNum[i] * (2^(frac_start-1-i))
        base *= 2; // Update base for the next bit
    }

    // Calculate the fractional part
    double fractionalPart = 0.0;
    base = 0.5; // This will represent 2^(-1), 2^(-2), ...
    
    for (int i = *frac_start; i < *size; i++) {
        fractionalPart += binaryNum[i] * base; // binaryNum[i] * (2^(frac_start-i))
        base /= 2; // Update base for the next bit
    }

    // Combine both parts
    *number = integerPart + fractionalPart;
}

// Function to convert binary array to decimal
void binary2decimal(int size, int* binaryNum, int* integer) {
    *integer = 0; // Initialize integer to 0
int temp = 1;
    // Iterate over the binary array
    for (int i = size; i >= 0; i--) {
        *integer = *integer + binaryNum[i]*temp; 
        temp = temp*2;
    }
}


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

// Function to shift the elements of an array to the right by k spaces (no wrap-around)
void shiftRight(int* arr, int size, int k) {
    if (size <= 0 || k <= 0) {
            std::cout << " shift of size was 0!"<< k << size<< "\n";
            return;
    } ; // No elements to shift

    // Normalize k in case it's larger than the array size
    if (k > size) {
        k = size;
    }

    // Shift elements to the right by k positions
    for (int i = size - 1; i >= k; i--) {
        arr[i] = arr[i - k];
    }

    // Fill the first k positions with zeros
    for (int i = 0; i < k; i++) {
        arr[i] = 0;
    }
}

void shiftLeft(int* arr, int size, int k) {
    if (size <= 0 || k <= 0) {
        std::cout << " k or size was 0! k/size: "<< k << "/"<< size<< "\n";
        return;
    }
    // Normalize k in case it's larger than the array size
    if (k > size) {
        k = size;
    }

    // Shift elements to the left by k positions
    for (int i = 0; i < size - k; i++) {
        arr[i] = arr[i + k];
    }

    // Fill the remaining positions with zeros
    for (int i = size - k; i < size; i++) {
        arr[i] = 0;
    }
}


// Function to print the array
void printArray(int* arr, int size, int frac_start) {
    // Printing the integer part
    for (int j = 0; j < frac_start; j++) {
        std::cout << arr[j];
    }
    std::cout << ".";  // Decimal point to separate integer and fractional part
    // Printing the fractional part
    for (int j = frac_start; j < size; j++) {
        std::cout << arr[j];
    }
    std::cout << std::endl;
}

// Function to reverse the array (helper function)
void reverseArray(int* arr, int start, int end) {
    while (start < end) {
        std::swap(arr[start], arr[end]);
        start++;
        end--;
    }
}


// Function to convert decimal (float) to binary
int dec2binary(float num, int* binaryNum, int* frac_start) {
    // Separate the integer and fractional parts
    int integer_part = (int)num; // Extract integer part
    float fractional_part = num - integer_part; // Extract fractional part
    
    int i = 0; // Counter for binary array

    // Convert the integer part to binary
    while (integer_part > 0) {
        binaryNum[i] = integer_part % 2;
        integer_part /= 2;
        i++;
    }

    // Reverse the integer part (to correct the order)
    reverseArray(binaryNum, 0, i - 1);

    // Store where the fractional part starts
    *frac_start = i; // Mark the position of the fractional part in the array

    // Convert the fractional part to binary
    while (fractional_part > 0 && i < 63) { // Limit precision to 63 bits
        fractional_part *= 2;
        int fractional_bit = (int)fractional_part;
        binaryNum[i] = fractional_bit;
        fractional_part -= fractional_bit;
        i++;
    }

    // Return the total size of the binary representation (integer + fractional part)
    return i;
}


// Funtion to convert binary to log using michell methodology
void antilog_bin(int size, int* binaryNum, int* frac_start ) {
    int integer;
    int temp;
    binary2decimal(*frac_start -1, binaryNum, &integer);
    for (int i = 0; i < *frac_start; i++){
        binaryNum[i] = 0;
    }
         binaryNum[ *frac_start -1] = 1;
    *frac_start = *frac_start + integer ;
}

// Function to generate a random floating-point number between 0 and 20000
float generateRandomFloat(float maxValue = 50) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<float> dist(1, maxValue);
    
    return dist(gen);
}

int main(int argc, char* argv[]) {
    float n1;
    double correct;

    // Array to store binary number (64 bits should be enough for typical floats)
    int binaryNum1[64] = {0}; // Initialize array with zeros

    double number;
    int frac_start1; // To store where the fractional part starts
    int size1;
    int fin = 1000;
    size1 = 64;

    for (int i = 0; i< fin; i++){

        n1 = generateRandomFloat();

        std::cout << "The antilogarithm of 2 of integer " << n1 <<"\n";
        dec2binary(n1, &binaryNum1[0], &frac_start1);

        antilog_bin(size1,&binaryNum1[0],&frac_start1);

        binaryToDouble( &size1, &frac_start1, &binaryNum1[0], &number);

        std::cout << "The antilogarithm of 2 of integer " << n1 << " is : " <<  number  << "\n";
        correct = std::pow(2, n1);
        std::cout << "The correct result is :" << correct << "\n";
        correct = correct - number;    // calculate the error (we could use double for better accuracy)
        std::cout << "The error between them is :" << correct << "\n";
        std::cout << std::endl;

        if(abs(correct) > number*0.061476){
            std::cout << "[ERROR]\n";
            break;
        }
        for (int i=0; i<size1; i++){
            binaryNum1[i] = 0 ; 
        }
    }
    return 0;
}