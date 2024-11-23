#include "ac_fixed.h"


// This function performs the piecewise aproximation to convert the fractional part of a float number to int logarithmic aproximate
// The input range is only the fraction part 0 <= f < 1 and it is divided in 8 equal spaces
void float2log_LUT(ac_fixed< W, I, true> &float_num){
    //std::cout << "The float part first three bit were : " <<float_num[I-1] << float_num[I-2] << float_num[I-3] <<"\n";

    ac_fixed< W, I, true> shift_float_num1;
    ac_fixed< W, I, true> comp_float_num1;
    ac_fixed< W, I, true> comp_float_num2;

    // Check if the first 3 bits are 000
    if ( float_num[I-1] == 0 &&  float_num[I-2] == 0 &&  float_num[I-3] == 0 ) {

        // Prepare first array (f>>1)
        shift_float_num1 = float_num>>1;

        // Prepare second array (~f>>3)
        comp_float_num1 = float_num>>3;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare third array (~f>>7)
        comp_float_num2 = float_num>>7;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products  
        // Here b is 0 (zero)              
        float_num = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2;

    }
    
    // Check if the first 3 bits are 001
    else if ( float_num[I-1] == 0 &&  float_num[I-2] == 0 &&  float_num[I-3] == 1 ) {

        // Prepare first array (f>>1)
        shift_float_num1 = float_num>>1;

        // Prepare second array (~f>>2)
        comp_float_num1 = float_num>>2;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare third array (~f>>6)
        comp_float_num2 = float_num>>6;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.0146484375);

    }
    // Check if the first 3 bits are 010
    else if ( float_num[I-1] == 0 &&  float_num[I-2] == 1 &&  float_num[I-3] == 0 ) {

        // Prepare first array (f>>2)
        shift_float_num1 = float_num>>2;

        // Prepare second array (~f>>3)
        comp_float_num1 = float_num>>3;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare third array (~f>>6)
        comp_float_num2 = float_num>>6;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.044921875);

    }
     // Check if the first 3 bits are 011
    else if ( float_num[I-1] == 0 &&  float_num[I-2] == 1 &&  float_num[I-3] == 1 ) {


        // Prepare first array (~f>>8)
        comp_float_num1 = float_num>>8;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + comp_float_num1 + ac_fixed<W, I, 0>(0.0888671875);

    }
     // Check if the first 3 bits are 100
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 0 &&  float_num[I-3] == 0 ) {

        // Prepare first array (f>>4)
        shift_float_num1 = float_num>>4;

        // Prepare second array (~f>>3)
        comp_float_num1 = float_num>>3;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare third array (~f>>7)
        comp_float_num2 = float_num>>7;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.1201171875);

    }
    // Check if the first 3 bits are 101
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 0 &&  float_num[I-3] == 1 ) {

        // Prepare first array (~f>>3)
        comp_float_num1 = float_num>>3;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare second array (~f>>6)
        comp_float_num2 = float_num>>6;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.1630859375);

    }
    // Check if the first 3 bits are 110
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 1 &&  float_num[I-3] == 0 ) {

        // Prepare first array (f>>4)
        shift_float_num1 = float_num>>4;

        // Prepare second array (~f>>2)
        comp_float_num1 = float_num>>2;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare third array (~f>>6)
        comp_float_num2 = float_num>>6;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.2099609375);

    }
    // Check if the first 3 bits are 111
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 1 &&  float_num[I-3] == 1 ) {

        // Prepare first array (~f>>2)
        comp_float_num1 = float_num >> 2;
        comp_float_num1 = ~comp_float_num1+ (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare second array (~f>>7)
        comp_float_num2 = float_num>>7;
        comp_float_num2 = ~comp_float_num2 + (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, true>(0.2578125);
    }
}

// We call this function to make the conversion from a float number to a logarithm 
void float2log(ac_fixed< W, I, true> &float_num ){

    bool one_found = false;
    int i;
    int one_place;

    // Find the leading one in the integer part
    for (i = W-1; i > (I-1); i--){
        if( float_num[i] == 1 && one_found == false){
            float_num[i] = 0; // remove the leading one
            one_found = true;
            one_place = i - I;
        }
    }

    // Shift the binary num right
    if(one_found == true){
        float_num >>= one_place;
    }

    //std::cout << "Before LUT " << float_num << "\n";
    // Run the peicewice approximation ( The integer part must me missing)
    float2log_LUT(float_num);
    //std::cout << "after LUT " << float_num << "\n";


    // Insert the new integer part
    if(one_found == true){
        float_num = float_num + ac_fixed<W, I, false>(one_place);
    }
}

