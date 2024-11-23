#include "ac_fixed.h"

// This function performs the piecewise aproximation to convert the fractional part of a logarithm to its float aproximate
// The input range is only the fraction part 0 <= f < 1 and it is divided in 8 equal spaces
void log2float_LUT(ac_fixed< W, I, true> &float_num){

    //std::cout << "The float part first three bit were : " <<float_num[I-1] << float_num[I-2] << float_num[I-3] <<"\n";

    ac_fixed< W, I, true> shift_float_num1;
    ac_fixed< W, I, true> comp_float_num1;
    ac_fixed< W, I, true> comp_float_num2;


    // Check if the first 3 bits are 000
    if ( float_num[I-1] == 0 &&  float_num[I-2] == 0 &&  float_num[I-3] == 0 ) {

        // Prepare first array (~f>>2)
        comp_float_num1 = float_num>>2;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare second array (~f>>5)
        comp_float_num2 = float_num>>5;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products  
        // Here b is 0 (zero)              
        float_num = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(1);

    }

    // Check if the first 3 bits are 001
    else if ( float_num[I-1] == 0 &&  float_num[I-2] == 0 &&  float_num[I-3] == 1 ) {

        // Prepare first array (f>>5)
        shift_float_num1 = float_num>>5;

        // Prepare second array (~f>>2)
        comp_float_num1 = float_num>>2;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare third array (~f>>7)
        comp_float_num2 = float_num>>7;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.9912109375);

    }
    // Check if the first 3 bits are 010
    else if ( float_num[I-1] == 0 &&  float_num[I-2] == 1 &&  float_num[I-3] == 0 ) {

        // Prepare first array (~f>>3)
        comp_float_num1 = float_num>>3;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare secont array (~f>>7)
        comp_float_num2 = float_num>>7;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.9716796875);

    }

    // Check if the first 3 bits are 011
    else if ( float_num[I-1] == 0 &&  float_num[I-2] == 1 &&  float_num[I-3] == 1 ) {

        // Prepare first array (~f>>4)
        comp_float_num1 = float_num>>4;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare secont array (f>>7)
        shift_float_num1 = float_num>>7;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1 + ac_fixed<W, I, 0>(0.94140625);

    }
    
    // Check if the first 3 bits are 011
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 0 &&  float_num[I-3] == 0 ) {

        // Prepare first array (f>>5)
        shift_float_num1 = float_num>>5;

        // Prepare second array (~f>>7)
        comp_float_num1 = float_num>>7;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1  + ac_fixed<W, I, 0>(0.90234375);

    }
    // Check if the first 3 bits are 101
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 0 &&  float_num[I-3] == 1 ) {

        // Prepare first array (~f>>3)
        comp_float_num1 = float_num>>3;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare second array (~f>>7)
        comp_float_num2 = float_num>>7;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.84375);

    }
    // Check if the first 3 bits are 110
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 1 &&  float_num[I-3] == 0 ) {

        // Prepare first array (f>>2)
        shift_float_num1 = float_num>>2;

        // Prepare second array (~f>>5)
        comp_float_num1 = float_num>>5;
        comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Prepare third array (~f>>7)
        comp_float_num2 = float_num>>7;
        comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

        // Add the products                
        float_num = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.7734375);

    }
    
    // Check if the first 3 bits are 111
    else if ( float_num[I-1] == 1 &&  float_num[I-2] == 1 &&  float_num[I-3] == 1 ) {

        // Here we need only shifts!

        // Prepare first array (~f>>2)
        shift_float_num1 = float_num >> 2;

        // Prepare second array (~f>>4)
        comp_float_num1 = float_num>>4;

        // Prepare third array (~f>>7)
        comp_float_num2 = float_num>>7;


        // Add the products                
        float_num = float_num  + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, true>(0.6787109375);
    }
}

// We call this function to make the conversion from a logarithm to a float number
void log2float(ac_fixed< W, I, true> &float_num ){

    int integer;
    ac_fixed< W, I, true> temp;
    temp.set_val<AC_VAL_0>();

    // store the integer
    integer=float_num.to_int();

    // remove the integer
    float_num.range<W-1,I>() = temp.range<W-1,I>();


    // run the LUT
    //std::cout << "Before LUT " << float_num << "\n";
    // Run the peicewice approximation ( The integer part must me missing)
    log2float_LUT(float_num);
    //std::cout << "after LUT " << float_num << "\n";

    // Shift the binary num Left integer times
    float_num <<= integer;
    
}

