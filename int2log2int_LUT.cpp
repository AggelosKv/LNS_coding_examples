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
void binaryAdd(int size1, int* binaryNum1, int size2, int* binaryNum2) {
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
double generateRandomFloat(double maxValue = 20000.0) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<double> dist(0.5, maxValue);
    
    return dist(gen);
}


// Driver program to test the above function
int main(int argc, char* argv[]) {
    float n1;

    // Array to store binary number (64 bits should be enough for typical floats)
    int binaryNum1[64] = {0}; // Initialize array with zeros
    int binaryNum2[64] = {0}; 
    int binaryNum3[64] = {0}; 

    int frac_start1, frac_start2, frac_start3; // To store where the fractional part starts
    int size1;
    int size2;
    int size3;
    double answer;
    double correct;
    bool error_flag = false;
    bool output_en = false;
    double error;
    
    output_en = true;
    // Number of loops
    int max_iterations = 10000;
    for (int k = 0; k < max_iterations; k++){
        // Generate and print a random floating-point number below 20000
     n1 = generateRandomFloat();
     error = n1 * 0.00130;
    std::cout << "Random floating-point number: " << n1 << std::endl;

    size1 = dec2binary(n1, &binaryNum1[0], &frac_start1);
    size2 = 0;

    // Output the binary number1 in correct order
    std::cout << "Binary of num1 " << n1 << " with "<<  frac_start1 << " bits for integer is: ";
    printArray(binaryNum1, size1, frac_start1);
    
    binary2log(size1, frac_start1,  &binaryNum1[0], &size2, &frac_start2,  &binaryNum2[0], &output_en );

    std::cout << "The final anwer is: ";
    printArray(binaryNum2, size2, frac_start2);

    log2binary(size2, frac_start2,  &binaryNum2[0], &size3, &frac_start3,  &binaryNum3[0], &output_en);

    binary2double(&size3, &frac_start3,  &binaryNum3[0], &answer);

    std::cout << std::setprecision(15) << "The decimal representation is: " << answer <<"\n";
    correct = n1;
    std::cout << "The correct result is :" << correct << "\n";
    correct = correct - answer;    // calculate the error (we could use double for better accuracy)
    std::cout << "The error between them is :" << correct << "\n";
    std::cout << std::endl;

        if (abs(correct) > error){
            std::cout << "The number :" << n1 << " produces wrong result! \n";
            std::cout << "The error should be less than :" << error <<" \n";
            error_flag = true;
            break;
        }
    }

    if (!error_flag){
        std::cout << "No errorus results where detected! \n";
    }

    return 0;
}
