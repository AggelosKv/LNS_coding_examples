#include <iostream>
#include <cmath>
#include <random>
#include <iomanip> // For std::setprecision

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

// Function to copy elements from one array to another
void copyArray(int* source, int size, int* destination) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i]; // Copy each element from source to destination
    }
}

// Function to convert a binary number to a double
void binary2double(int* size, int* frac_start, int* binaryNum, double* number) {
    // Validate inputs
    if (*size <= 0 || *frac_start < 0 || *frac_start > *size) {
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

// Function to add two binary numbers binaryNum1 and binaryNum2
int binaryAdd(int size1, int* binaryNum1, int size2, int* binaryNum2) {
    int carry = 0;

    // Find the maximum size for looping from the end of both arrays
    int maxSize = (size1 > size2) ? size1 : size2;

    // Step 1: Start adding bits from the end of the arrays (most significant bits)
    for (int i = maxSize - 1; i >= 0; i--) {
        int bit1 = (i >= size1) ? 0 : binaryNum1[i];  // Use 0 if binaryNum1 is smaller
        int bit2 = (i >= size2) ? 0 : binaryNum2[i];  // Use 0 if binaryNum2 is smaller

        int sum = bit1 + bit2 + carry;
        carry = sum / 2;           // Update carry for the next most significant bit

        if (i < size1) {
            binaryNum1[i] = abs(sum % 2); // Store the result in binaryNum1 if within bounds
        }
    }
    //printArray(binaryNum1, size1, 0);

    return (carry);
    // // Handle overflow if carry is still set after the loop
    // if (carry) {
    //     // Overflow handling depends on your needs (e.g., increasing array size or ignoring it)
    //     std::cout << "Overflow occurred!" << std::endl;
    // }
}

// Function to calculate the two's complement of a binary number
void complementCalc2(int size1, int* binaryNum1) {
    // Step 1: Calculate one's complement by flipping all bits
    for (int i = 0; i < size1; i++) {
        binaryNum1[i] = (binaryNum1[i] == 0) ? 1 : 0; // Flip the bits
    }

    // Step 2: Create an array for the binary representation of "1"
    int binaryOne[64] = {0}; // Adjust the size as necessary
    binaryOne[size1-1] = 1;        // The binary representation of 1 is just {1}

    // Step 3: Use binaryAdd to add 1 to the one's complement
    binaryAdd(size1, binaryNum1,size1, binaryOne);

}


// Function to convert an integer to binary and store it in an array
int int2binary(int num, int* binaryArr) {
    int i = 0;
    while (num > 0) {
        binaryArr[i] = num % 2;
        num /= 2;
        i++;
    }
    // Reverse the array to get the correct binary representation
    for (int j = 0; j < i / 2; j++) {
        std::swap(binaryArr[j], binaryArr[i - j - 1]);
    }
    return i; // Return the size of the binary representation
}

// Function to shift the elements of an array to the right by k spaces (no wrap-around)
void shiftRight(int* arr, int size, int k) {
    if (size <= 0 || k <= 0) return; // No elements to shift

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
    if (size <= 0 || k <= 0) return; // No elements to shift

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




// Function to reverse the array (helper function)
void reverseArray(int* arr, int start, int end) {
    while (start < end) {
        std::swap(arr[start], arr[end]);
        start++;
        end--;
    }
}

// Function to perform LUT operations
void LUT_log(int* size1, int* binaryNum1 , bool* output_en) {
    // Check if the first three bits are 000
    if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 0 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }
        
        
        int shiftedNum1[64]; // Adjust size as necessary
        
        // prepare first array (f>>1)
        copyArray(binaryNum1, *size1, shiftedNum1); 
        shiftRight(shiftedNum1, *size1+1, 1);
        
        //prepare second array (~f>>3)
        int complementNum1[64]; 
        copyArray(binaryNum1, *size1, complementNum1); 
        shiftRight(complementNum1, *size1+3, 3);
        complementCalc2(*size1+3, complementNum1);


        // add first 2 arrays
        binaryAdd(*size1+3, complementNum1, *size1,  shiftedNum1);

        //prepare third array (~f>>7)
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        shiftRight(complementNum2, *size1+7, 7);
        complementCalc2(*size1, complementNum2);
        // Shift the two's complement right by 7

        // here b constant is 0
        // Add the result of step 3 to the shifted complementNum2
                
        binaryAdd(*size1+7, complementNum2, *size1+3, complementNum1);

        binaryAdd(*size1,binaryNum1 , *size1+7,complementNum2 );

    }
    //check if the bits are 001
    else if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 0 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }

        // Step 1: Shift the original binaryNum1 right by 1
        int shiftedNum1[64]; // Adjust size as necessary
        copyArray(binaryNum1, *size1, shiftedNum1); 
        shiftRight(shiftedNum1, *size1+1, 1);

        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+2, 2);
        complementCalc2(*size1+2, complementNum1);

        // Step 3: Add shiftedNum1 to shifted complementNum1
        binaryAdd(*size1+2, complementNum1, *size1+1, shiftedNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 

        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1 , 6);
        complementCalc2(*size1+6, complementNum2);

        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+6, complementNum2, *size1+2,complementNum1 );

        // create b
        int b_constant[10] = {0,0,0,0,0,0,1,1,1,1};
        binaryAdd(*size1+6, complementNum2, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+6, complementNum2);

    }
     //check if the bits are 010
    else if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 1 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }

        // Step 1: Shift the original binaryNum1 right by 1
        int shiftedNum1[64]; // Adjust size as necessary
        copyArray(binaryNum1, *size1, shiftedNum1); 
        shiftRight(shiftedNum1, *size1+2, 2);

        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+3, 3);
        complementCalc2(*size1+3, complementNum1);

        // Step 3: Add shiftedNum1 to shifted complementNum1
        binaryAdd(*size1+3, complementNum1, *size1+2, shiftedNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1+6, 6);
        complementCalc2(*size1+6, complementNum2);



        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+6, complementNum2, *size1+3, complementNum1);


        // create b
        int b_constant[10] = {0,0,0,0,1,0,1,1,1,0};
        binaryAdd(*size1+6, complementNum2, 10, b_constant);
        
        binaryAdd(*size1, binaryNum1, *size1+6, complementNum2);

    }
         //check if the bits are 011
    else if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 1 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        
        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1+8, 8);
        complementCalc2(*size1 + 8, complementNum2);

        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+8, binaryNum1, *size1+8, complementNum2);

        // create b
        int b_constant[10] = {0,0,0,1,0,1,1,0,1,1};
        
        binaryAdd(*size1, binaryNum1, 10, b_constant);

    }
         //check if the bits are 100
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 0 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }

        // Step 1: Shift the original binaryNum1 right by 1
        int shiftedNum1[64]; // Adjust size as necessary
        copyArray(binaryNum1, *size1, shiftedNum1); 
        shiftRight(shiftedNum1, *size1+4, 4);

        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+3, 3);
        complementCalc2(*size1+3, complementNum1);

        // Step 3: Add shiftedNum1 to shifted complementNum1
        binaryAdd(*size1+4, shiftedNum1, *size1+3, complementNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1+7, 7);
        complementCalc2(*size1+7, complementNum2);

        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+7, complementNum2, *size1+4, shiftedNum1);


        // create b
        int b_constant[10] = {0,0,0,1,1,1,1,0,1,1};
        binaryAdd(*size1+7, complementNum2, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+7, complementNum2);

    }
             //check if the bits are 101
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 0 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+3, 3);
        complementCalc2(*size1+3, complementNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1+6, 6);
        complementCalc2(*size1+6, complementNum2);

        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+6, complementNum2, *size1+3,complementNum1 );


        // create b
        int b_constant[10] = {0,0,1,0,1,0,0,1,1,1};
        binaryAdd(*size1+6, complementNum2, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+6, complementNum2);

    }
                 //check if the bits are 110
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 1 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        // Step 1: Shift the original binaryNum1 right by 1
        int shiftedNum1[64]; // Adjust size as necessary
        copyArray(binaryNum1, *size1, shiftedNum1); 
        shiftRight(shiftedNum1, *size1+4, 4);

        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+2, 2);
        complementCalc2(*size1+2, complementNum1);

        // Step 3: Add shiftedNum1 to shifted complementNum1
        binaryAdd(*size1+4, shiftedNum1, *size1+2, complementNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1+6, 6);
        complementCalc2(*size1+6, complementNum2);

        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+6, complementNum2, *size1+4, shiftedNum1);



        int b_constant[10] = {0,0,1,1,0,1,0,1,1,1};
        binaryAdd(*size1+6, complementNum2, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+6, complementNum2);

    }
                     //check if the bits are 111
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 1 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+2, 2);
        complementCalc2(*size1+2, complementNum1);



        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1+7, 7);
        complementCalc2(*size1+7, complementNum2);



        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+7, complementNum2, *size1+2, complementNum1);


        // create b
        int b_constant[10] = {0,1,0,0,0,0,1,0,0,0};
        binaryAdd(*size1 + 7, complementNum2, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1 + 7, complementNum2);

    }
}


// Function to perform LUT operations
void LUT_antilog(int* size1, int* binaryNum1 , bool* output_en) {
    // Check if the first three bits are 000
    if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 0 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }
        
        
        //prepare second array (~f>>2)
        int complementNum1[64]; 
        copyArray(binaryNum1, *size1, complementNum1); 
        shiftRight(complementNum1, *size1+2, 2);
        complementCalc2(*size1+2, complementNum1);

        //prepare third array (~f>>5)
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        shiftRight(complementNum2, *size1+5, 5);
        complementCalc2(*size1, complementNum2);
        // Shift the two's complement right by 5

        // here b constant is 0
        // Add the result of step 3 to the shifted complementNum2
                
        binaryAdd(*size1+5, complementNum2, *size1+2, complementNum1);

        binaryAdd(*size1,binaryNum1 , *size1+5, complementNum2 );

    }
    //check if the bits are 001
    else if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 0 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }

        // Step 1: Shift the original binaryNum1 right by 1
        int shiftedNum1[64]; // Adjust size as necessary
        copyArray(binaryNum1, *size1, shiftedNum1); 
        shiftRight(shiftedNum1, *size1+5, 5);

        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+2, 2);
        complementCalc2(*size1+2, complementNum1);

        // Step 3: Add shiftedNum1 to shifted complementNum1
        binaryAdd(*size1+5, shiftedNum1, *size1+2,  complementNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int shiftedNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, shiftedNum2); 

        // Shift the two's complement right by 7
        shiftRight(shiftedNum2, *size1+7 , 7);

        // Add the result of step 3 to the shifted shiftedNum2
        binaryAdd(*size1+7, shiftedNum2, *size1+5, shiftedNum1 );

        // create b
        int b_constant[11] = {1,1,1,1,1,1,0,1,1,1};
        binaryAdd(*size1+7, shiftedNum2, 11, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+7, shiftedNum2);

    }
     //check if the bits are 010
    else if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 1 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 3
        shiftRight(complementNum1, *size1+3, 3);
        complementCalc2(*size1+3, complementNum1);

        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        // Shift the two's complement right by 7
        shiftRight(complementNum2, *size1+7, 7);
        complementCalc2(*size1+7, complementNum2);



        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+7, complementNum2, *size1+3, complementNum1);


        // create b
        int b_constant[11] = {1,1,1,1,1,0,0,0,1,1};
        binaryAdd(*size1+7, complementNum2, 11, b_constant);
        
        binaryAdd(*size1, binaryNum1, *size1+7, complementNum2);

    }
         //check if the bits are 011
    else if (*size1 >= 3 && binaryNum1[0] == 0 && binaryNum1[1] == 1 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }



        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 4
        shiftRight(complementNum1, *size1+4, 4);
        complementCalc2(*size1+4, complementNum1);

        int shiftedNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, shiftedNum2); 
        // Shift the two's complement right by 7
        shiftRight(shiftedNum2, *size1+7, 7);

        // Add the result of step 3 to the shifted shiftedNum2
        binaryAdd(*size1+7, shiftedNum2, *size1+3, complementNum1);


        // create b
        int b_constant[10] = {1,1,1,1,0,0,0,1,0,0};
        binaryAdd(*size1+7, shiftedNum2, 10, b_constant);
        
        binaryAdd(*size1+7, binaryNum1, *size1+7, shiftedNum2);

    }
         //check if the bits are 100
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 0 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }

        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 7
        shiftRight(complementNum1, *size1+7, 7);
        complementCalc2(*size1+7, complementNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int shiftedNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, shiftedNum1); 
        // Shift the two's complement right by 5
        shiftRight(shiftedNum1, *size1+5, 5);

        // Add the result of step 5 to the shifted complementNum2
        binaryAdd(*size1+7, complementNum1, *size1+5,shiftedNum1 );

        // create b
        int b_constant[10] = {1,1,1,0,0,1,1,1,0};
        binaryAdd(*size1+7, complementNum1, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+7, complementNum1);

    }
             //check if the bits are 101
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 0 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 7
        shiftRight(complementNum1, *size1+7, 7);
        complementCalc2(*size1+7, complementNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int shiftedNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, shiftedNum1); 
        // Shift the two's complement right by 3
        shiftRight(shiftedNum1, *size1+3, 3);

        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+7, complementNum1, *size1+3,shiftedNum1 );


        // create b
        int b_constant[10] = {1,1,0,1,1,0,0,0,0,0};
        binaryAdd(*size1+7, complementNum1, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+7, complementNum1);

    }
                 //check if the bits are 110
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 1 && binaryNum1[2] == 0) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        // Step 1: Shift the original binaryNum1 right by 1
        int shiftedNum1[64]; // Adjust size as necessary
        copyArray(binaryNum1, *size1, shiftedNum1); 
        shiftRight(shiftedNum1, *size1+2, 2);

        // Step 2: Calculate the two's complement of binaryNum1
        int complementNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum1); 
        // Shift the two's complement right by 2
        shiftRight(complementNum1, *size1+5, 5);
        complementCalc2(*size1+5, complementNum1);

        // Step 3: Add shiftedNum1 to shifted complementNum1
        binaryAdd(*size1+5, complementNum1, *size1+2, shiftedNum1);

        // Step 4: Calculate the two's complement of binaryNum1 again for shifting
        int complementNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, complementNum2); 
        // Shift the two's complement right by 6
        shiftRight(complementNum2, *size1+7, 7);
        complementCalc2(*size1+7, complementNum2);

        // Add the result of step 3 to the shifted complementNum2
        binaryAdd(*size1+7, complementNum2, *size1+5, complementNum1);

        int b_constant[10] = {1,1,0,0,0,1,1,0,0,0};
        binaryAdd(*size1+7, complementNum2, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1+7, complementNum2);

    }
                     //check if the bits are 111
    else if (*size1 >= 3 && binaryNum1[0] == 1 && binaryNum1[1] == 1 && binaryNum1[2] == 1) {

        if (*output_en == true){
             std::cout << "The float part first three bit were : " << binaryNum1[0] << binaryNum1[1] << binaryNum1[2] <<"\n";
        }


        int shiftedNum1[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, shiftedNum1); 
        // Shift the two's complement right by 2
        shiftRight(shiftedNum1, *size1+2, 2);


        int shiftedNum2[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, shiftedNum2); 
        // Shift the two's complement right by 4
        shiftRight(shiftedNum2, *size1+4, 4);
        binaryAdd(*size1+4, shiftedNum2, *size1+2, shiftedNum1);

        int shiftedNum3[64]; // To hold the two's complement
        copyArray(binaryNum1, *size1, shiftedNum3); 
        // Shift the two's complement right by 7
        shiftRight(shiftedNum3, *size1+7, 7);

        binaryAdd(*size1+7, shiftedNum3, *size1+4, shiftedNum2);

        // create b
        int b_constant[10] = {1,0,1,0,1,1,0,1,1,1};
        binaryAdd(*size1 + 7, shiftedNum3, 10, b_constant);
        binaryAdd(*size1, binaryNum1, *size1 + 7, shiftedNum3);

    }
}

// Function to convert decimal (float) to binary
void dec2binary_MADAM(int* num, int* binaryNum, int num_of_bits) {
    // Separate the integer and fractional parts
    int integer_part = *num; // Extract integer part
    bool is_neg = false;

    if (integer_part < 0){
        is_neg = true;
        integer_part = integer_part *(-1);
    }


    int i = 0; // Counter for binary array
    int array_size;

    // Convert the integer part to binary
    while (integer_part > 0) {
        binaryNum[i] = integer_part % 2;
        integer_part /= 2;
        i++;
    }
    array_size=i ;


    // Reverse the integer part (to correct the order)
    reverseArray(binaryNum, 0, i - 1);


    for (int i = 0; i < array_size +1; i++) {
        binaryNum[num_of_bits - i ] = binaryNum[array_size - i ];
        binaryNum[array_size - i] = 0;
    }

    if (is_neg){
        binaryNum[0] = 1;
    }
}


void binary2log(int size1, int frac_start1, int* binaryNum1, int* size2, int* frac_start2, int* binaryNum2 , bool* output_en) {
    int index = 0; // Keeps track of current position in binaryNum2

    // Convert frac_start1 to binary and store in binaryNum2
    int tempBinary[32]; // Temporary array to hold the binary representation of frac_start1
    *frac_start2 = int2binary(frac_start1-1, tempBinary); // Convert frac_start1 to binary

    // Copy the binary representation of frac_start1 to binaryNum2
    for (int i = 0; i < *frac_start2; i++) {
        binaryNum2[i] = tempBinary[i];
    }


    // // remove the 'characteristic'
    // binaryNum1[0] = 0;

    shiftLeft(binaryNum1, size1+1, 1);

    if (*output_en == true){
    std::cout << "The float binary that we will transform with the LUT is : ";
    printArray(binaryNum1, size1,0);
    }

    LUT_log(&size1, binaryNum1, output_en);
    
    if (*output_en == true){
    std::cout << "Binary of num1 with "<<  *frac_start2 << " bits for integer after the LUT is: ";
    printArray(binaryNum1, size1, *frac_start2);
    }

    *size2 = *frac_start2 + size1;
    // Copy the binary representation of frac_start1 to binaryNum2
    for (int i = 0; i < size1; i++) {
        binaryNum2[i+*frac_start2 ] = binaryNum1[i];
    }

}

void log2binary(int size1, int frac_start1, int* binaryNum1, int* size2, int* frac_start2, int* binaryNum2 , bool* output_en){
    double  answer;
    int shift;

    // Create the value of shifting for the fraction part
    binary2double(&frac_start1, &frac_start1,  &binaryNum1[0], &answer);

    std::cout << "The fraction part will be shifted left "<< answer << " times.\n";

    // if (answer == 0)
    //     *frac_start2 = (int)(answer);
    // else
        *frac_start2 = (int)(answer + 1);  // this is the new frac_start2 variable

    std::cout << "The float binary that we will transform with the LUT is : ";

    *size2 = size1 -frac_start1 ;
    for (int i = 0; i < (*size2+1); i++) {
        binaryNum2[i] = binaryNum1[i+frac_start1];
    }
    printArray(binaryNum2, *size2,0);

    LUT_antilog(size2, binaryNum2, output_en);

    if ( *output_en == true){
        std::cout << "Binary of num1 with "<<  *frac_start2 << " bits for integer after the LUT is: ";
        printArray(binaryNum2, *size2, 0);
    }

    if (*frac_start2 !=0){

        *size2 =*size2+1;
        shiftRight(binaryNum2, *size2, 1);
        binaryNum2[0] = 1; // add 1 to the fraction part of the binaryNum1 by writing one to the first position to the array and then coping the rest of the array below that
    }

    if ( *output_en == true){
        std::cout << "Binary of final num1 with "<<  *frac_start2 << " bits after the antilogarithmic method is: ";
        printArray(binaryNum2, *size2, *frac_start2);
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



// Function to generate a random floating-point number between 0 and 20000
double generateRandomFloat(double maxValue = 127) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<double> dist(-maxValue, maxValue);
    
    return dist(gen);
}

void LNS_MAC_U(int size, int* gama, int* binaryNum1, int* binaryNum2, int* result,int* result_size,int* fractionstart, bool* output_en){

    bool sign;
    int tempBinary[size] = {0}; 
    int fraction_num1[size] = {0};
    int fraction_num2[size] = {0}; 


    int newsize = size +1; 
    int carry = 0;
    double quontent;
    double remainer;
    int temp_size = *gama - 1;
    int fractional_part =0;

    // create sign using xor
    sign = binaryNum1[0] ^ binaryNum2[0];

    if (*output_en == true){
    std::cout << "The final sign is : " << sign << "\n";
    }

    // remove the sign to do the addition
    binaryNum1[0] = 0;
    binaryNum2[0] = 0;

    if ( *output_en == true){
        std::cout << "The binary of num1 is : \n";
        printArray(&binaryNum1[0], size, size-*gama );
        std::cout << "The binary of num2 is : \n";
        printArray(&binaryNum2[0], size, size-*gama );
    }

    // Use LUT table in the start of the addition to reduce error

    // first split the fraction part from the number if the fraction exists
    if(gama != 0 ){
        for (int i = 0; i < *gama; i++ ){
            if(size - *gama + i >= size){
                break;
            }
            fraction_num1[i] = binaryNum1[size - *gama + i];
            fraction_num2[i] = binaryNum2[size - *gama + i];
            binaryNum1[size - *gama + i] = 0;
            binaryNum2[size - *gama + i] = 0;
        }
    }
    LUT_log(&size,&fraction_num1[0], output_en);
    LUT_log(&size,&fraction_num2[0], output_en);

    // return fraction on the numbers
    shiftRight(fraction_num1,size, (size - *gama) );
    binaryAdd(size, binaryNum1, size, fraction_num1);

    shiftRight(fraction_num2,size, (size - *gama) );
    binaryAdd(size, binaryNum2, size, fraction_num2);

    if ( *output_en == true){
        std::cout << "The binary of num1 after the LUT is : \n";
        printArray(&binaryNum1[0], size, size-*gama );
        std::cout << "The binary of num2 after the LUT is : \n";
        printArray(&binaryNum2[0], size, size-*gama );
    }

    // Addition of the two exponents
    carry = binaryAdd(size, binaryNum1, size, binaryNum2);

    // make the array bigger
    shiftRight(&binaryNum1[0], newsize , 1);
    binaryNum1[1] = carry;
    binaryNum1[0] = 0;

    if ( *output_en == true){
        std::cout << "The binary of addition is : \n";
        printArray(&binaryNum1[0], newsize, newsize );
    }

    // split to the quontent and remainer part of the e/gama
    for (int i = 0; i < *gama; i++){
        tempBinary[*gama - i -1] = binaryNum1[newsize - i -1];
    }

    shiftRight(&binaryNum1[0], newsize , *gama);
    binary2double(&newsize, &newsize, &binaryNum1[0], &quontent);

    binary2double(gama, gama, &tempBinary[0], &remainer);
    if ( *output_en == true){
        std::cout << "The remainer part has the value of "<< remainer <<" or else is the binary: \n";
        printArray(&tempBinary[0], *gama, *gama );
        std::cout << "The quontent part has the value of "<< quontent <<" or else is the binary : \n";
        printArray(&binaryNum1[0], newsize, newsize );
    }


    *result_size = (int)pow(2,quontent);

    if ( *output_en == true){
        std::cout << "The result size is :"<< *result_size <<" \n";
    }


    // Make the shifting by coping the old array in the right place of the answer
    // this should make the right output for different gama
    // if the remainer is zero we should write 1 on its place
    if(remainer != 0){
        if(*gama < *result_size){
            
            for (int i=0; i < *gama; i++  ){
                result[i] = tempBinary[i];
            }
            fractional_part = *result_size;
        }
        else{
            fractional_part = *result_size;
            *result_size = *gama;
            copyArray(&tempBinary[0], newsize, &result[0] );

        }
    }
    else{
        if(*gama < *result_size){
            
            result[0] = 1;
            fractional_part = quontent +1;
            
        }
        else{
            fractional_part = *result_size;
            *result_size = *gama;
            result[0] =1;
        }

    }


    if ( *output_en == true){
        std::cout << "The shift gives a product of : \n";
        printArray(&result[0], *result_size, fractional_part );
    }


        shiftRight(&result[0], *result_size ,1);
        *result_size = *result_size + 1;
        fractional_part = fractional_part +1;
    // if the sign is 1 find the 2's complement
    if (sign == 1){
        if ( *output_en == true){
            std::cout << "The sign was negative! \n";
        }
        complementCalc2(*result_size, &result[0]); 
    }

    if ( *output_en == true){
        std::cout << "The final result is: \n";
        printArray(&result[0], *result_size, fractional_part );
    }

*fractionstart = fractional_part;
}

// Function to convert a binary array to a signed floating-point number
void binary2signed(int* binaryarray, int size, int* fractionstart, float* result) {
  if (size <= 0 || *fractionstart < 0 || *fractionstart > size || result == nullptr) {
        *result = 0.0f;
        return;
    }

    // Step 1: Determine if the number is negative (using the sign bit)
    bool isNegative = binaryarray[0] == 1;

    // Step 2: Calculate the integer part
    int integerPart = 0;
    for (int i = 0; i < *fractionstart; ++i) {
        integerPart = (integerPart << 1) | binaryarray[i];
    }

    // If the number is negative, convert from two's complement to a negative integer
    if (isNegative) {
        // Perform two's complement conversion
        integerPart = integerPart - (1 << *fractionstart);
    }

    // Step 3: Calculate the fractional part
    float fractionalPart = 0.0f;
    for (int i = *fractionstart; i < size; ++i) {
        if (binaryarray[i] == 1) {
            fractionalPart += std::pow(2, -(i - *fractionstart + 1));
        }
    }

    // Step 4: Combine the integer and fractional parts
    *result = static_cast<float>(integerPart) + fractionalPart;
}


// Driver program to test the above function
int main(int argc, char* argv[]) {
    int n1,n2;

    int gama = 3;   // NOTE : gama in reality is pow(2,gama) , we input it like this for simpicity

    bool output_en = false;
    const int num_of_bits = 8;
    const int answer = 2048;
    int result_size;
    int fraction_start;
    float final_result;
    double n1_float, n2_float;

    // Array to store binary number (64 bits should be enough for typical floats)
    int binaryNum1[num_of_bits] = {0}; // Initialize array with zeros
    int binaryNum2[num_of_bits] = {0}; 

    int result[answer] = {0}; 


     n1 = (int)generateRandomFloat(10);
     n2 = (int)generateRandomFloat(10);

    output_en = true;

    if (output_en == true){
        std::cout << "Random intgers are: " << n1 << " and " << n2 << std::endl;
    }

    dec2binary_MADAM(&n1, &binaryNum1[0], num_of_bits);

    dec2binary_MADAM(&n2, &binaryNum2[0], num_of_bits);



    if (output_en == true){
        std::cout << "Binary of num1 " << n1 << " with "<<  num_of_bits << " bits is gama with the value of " << pow(2,gama) << " is :\n";
        printArray(&binaryNum1[0], num_of_bits, gama);
        std::cout << "Binary of num2 " << n2 << " with "<<  num_of_bits << " bits is gama with the value of " << pow(2,gama) << " is :\n";
        printArray(&binaryNum2[0], num_of_bits, gama);
    }

    LNS_MAC_U(num_of_bits, &gama, &binaryNum1[0],&binaryNum2[0],&result[0],&result_size,&fraction_start, &output_en );
    binary2signed(&result[0],result_size,&fraction_start,&final_result);
    n1_float = (double)n1/pow(2,gama);
    n2_float = (double)n2/pow(2,gama);
    std::cout << "The result is :" <<  final_result <<" and the correct result is : "<< ((n1*n2)/abs(n1*n2))*pow(2,abs(n1_float)+abs(n2_float)) << " \n";

    return 0;
}