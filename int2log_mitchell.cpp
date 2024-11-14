#include <cstdlib>
#include <iostream>
#include <cmath>
#include <random>
#include <iomanip> // For std::setprecision

//template<int N>

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

// Funtion to convert binary to log using michell methodology
int binary2log(int size, int* binaryNum, int* integer, float* fractal ) {
    *integer = size - 1;
    *fractal = 0;
    float temp = 0.5;
    for (int i = (size - 2); i>=0; i-- ){
       *fractal = binaryNum[i] * temp + *fractal ;
       temp = temp / 2;
    }
    return *integer;
}

// Function to generate a random floating-point number between 0 and 20000
float generateRandomFloat(float maxValue = 20000) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<float> dist(1, maxValue);
    
    return dist(gen);
}

// Driver program to test the above function
int main(int argc, char* argv[]) {
    int n = 137;

    // Array to store binary number (32 bits should be enough for typical integers)
    int binaryNum[32] = {0}; // Initialize array with zeros
    
    // Call the function with pointer to binaryNum
    int size;
    int fin = 1000;
    for (int i = 0; i< fin; i++){
        n = (int) abs(generateRandomFloat());

        size = decToBinary(n, &binaryNum[0]);
        
        // Output the binary number in reverse order
        std::cout << "Binary of " << n << " is: ";
        for (int j = size - 1; j >= 0; j--) {
            std::cout << binaryNum[j];
        }
        std::cout << "\n";

        int integer;
        float fractal;
        binary2log(size,&binaryNum[0],&integer,&fractal);

        fractal = integer + fractal;

        std::cout << "The logarithm of 2 of integer " << n << " is : " <<  fractal  << "\n";
        float correct = log2(n);
        std::cout << "The correct result is :" << correct << "\n";
        correct = correct - fractal;    // calculate the error (we could use double for better accuracy)
        std::cout << "The error between them is :" << correct << "\n";
        std::cout << std::endl;
        if(abs(correct) > 0.08639){
            std::cout << "[ERROR]\n";
            break;
        }
    }



    return 0;
}