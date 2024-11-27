#ifndef FLOAT_AND_LOG_OP_H
#define FLOAT_AND_LOG_OP_H

#include "ac_fixed.h"


template<int W,int I,bool T>
class fast_log{
    public:

        typedef ac_fixed<W,I,T> num_t;
        typedef ac_int<1,false> sign_t;

        num_t  num;
        sign_t sign;    // 1'b0 positive, 1'b1 negative

        // Constructors
        fast_log() {};

        // Constactor from float
        fast_log(const float &in) {
            this->operator=(in);
        }

        // Constractor from AC_INT
        fast_log(const ac_int<I,true> &in) {
            this->operator=(in);
        }
        // Copy constactor
        fast_log(const fast_log<W,I,T> &in) {
            this->operator=(in);
        }
        // De-constactor
        ~fast_log() {};

        void set(sign_t sign_s, num_t num_s) {
            sign = sign_s;
            num = num_s;
        }



        float to_float() {
            
            if (W <= 0 || I < 0 || I > W) {
                return (float(0)); // Invalid input; set to 0
            }

            // Calculate the integer part
            float integerPart = 0.0;
            float base = 1.0; // This will represent 2^0, 2^1, 2^2, ...
            float final_num;
            

            for (int i = I ; i <= W-1; i++) {
                integerPart += num[i] * base; // binaryNum[i] * (2^(frac_start-1-i))
                base *= 2; // Update base for the next bit
            }

            // Calculate the fractional part
            float fractionalPart = 0.0;
            base = 0.5; // This will represent 2^(-1), 2^(-2), ...
            
            for (int i = I-1; i > 0; i--) {
                fractionalPart += num[i] * base; // binaryNum[i] * (2^(frac_start-i))
                base /= 2; // Update base for the next bit
            }

            // Combine both parts
            final_num = integerPart + fractionalPart;
        
            if (sign == 1 ){
                final_num = final_num * -1;
            }

            return  final_num;
        }


        void print() const {
            std::cout << "fast_log(" << num << ")" << std::endl;
        }

        // We call this function to make the conversion from a float number to a logarithm 
        void float2log(const fast_log<W, I, T> float_num , fast_log<W, I, T> &float_num_out1){

            bool one_found = false;
            int i;
            int one_place;
            
            ac_fixed<W, I, true> temp_num = float_num.num;
            ac_fixed<W, I, true> float_num_temp_out;

            // Find the leading one in the integer part
            for (i = W-1; i > (I-1); i--){
                if( temp_num[i] == 1 && one_found == false){
                    temp_num[i] = 0; // remove the leading one
                    one_found = true;
                    one_place = i - I;
                }
            }

            // Shift the binary num right
            if(one_found == true){
                temp_num >>= one_place;
            }

            //std::cout << "Before LUT " << float_num << "\n";
            // Run the peicewice approximation ( The integer part must me missing)
            LUT_float2log(temp_num, float_num_temp_out);
            //std::cout << "after LUT " << float_num << "\n";


            // Insert the new integer part
            if(one_found == true){
                float_num_out1.num = float_num_temp_out + ac_fixed<W, I, true>(one_place);
            }
            else{
                float_num_out1.num = float_num_temp_out;
            }
        }


    
        
         // This function performs the piecewise aproximation to convert the fractional part of a float number to int logarithmic aproximate
        // The input range is only the fraction part 0 <= f < 1 and it is divided in 8 equal spaces
        void LUT_float2log(const ac_fixed<W, I, true> input, ac_fixed<W, I, true> &float_num_out){
            //std::cout << "The float part first three bit were : " <<float_num[I-1] << float_num[I-2] << float_num[I-3] <<"\n";

            ac_fixed<W, I, true> shift_float_num1;
            ac_fixed<W, I, true> comp_float_num1;
            ac_fixed<W, I, true> comp_float_num2;
            ac_fixed<W, I, true> float_num = input;
            // Check if the first 3 bits are 000
            if (  float_num.template slc<4>(W-I-3) == 0 ) {

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
            else if (  float_num.template slc<4>(W-I-3) == 1 ) {

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
            else if (  float_num.template slc<4>(W-I-3) == 2 ) {

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
            else if (  float_num.template slc<4>(W-I-3) == 3 ) {


                // Prepare first array (~f>>8)
                comp_float_num1 = float_num>>8;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products                
                float_num = float_num + comp_float_num1 + ac_fixed<W, I, 0>(0.0888671875);

            }
            // Check if the first 3 bits are 100
            else if (  float_num.template slc<4>(W-I-3) == 4 ) {

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
            else if (  float_num.template slc<4>(W-I-3) == 5 ) {

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
            else if (  float_num.template slc<4>(W-I-3) == 6 ) {

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
            else if (  float_num.template slc<4>(W-I-3) == 7 ) {

                // Prepare first array (~f>>2)
                comp_float_num1 = float_num >> 2;
                comp_float_num1 = ~comp_float_num1+ (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Prepare second array (~f>>7)
                comp_float_num2 = float_num>>7;
                comp_float_num2 = ~comp_float_num2 + (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products                
                float_num = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, true>(0.2578125);
            }
            float_num_out = float_num;
        }

        // We call this function to make the conversion from a logarithm to a float number
        void log2float(const fast_log<W, I, T> input, fast_log<W, I, T> &float_num_out ){

            int integer;
            ac_int< W - I, true> temp;
            ac_fixed<W, I, true> float_num_temp_out;
            temp = 0;
            ac_fixed<W, I, true> float_num = input.num;

            // store the integer
            integer=float_num.to_int();

            // clear the integer part
            float_num.set_slc(I, temp);  // Set the integer part to zero

            // run the LUT
            //std::cout << "Before LUT " << float_num << "\n";
            // Run the peicewice approximation ( The integer part must me missing)
            LUT_log2float(float_num, float_num_temp_out);
            //std::cout << "after LUT " << float_num << "\n";

            // Shift the binary num Left integer times
            float_num_temp_out <<= integer;
            
            float_num_out.num = float_num_temp_out;
            
        }

        // This function performs the piecewise aproximation to convert the fractional part of a logarithm to its float aproximate
        // The input range is only the fraction part 0 <= f < 1 and it is divided in 8 equal spaces
        void LUT_log2float(const ac_fixed<W, I, true> input, ac_fixed<W, I, true> &float_num_out){

            //std::cout << "The float part first three bit were : " <<float_num[I-1] << float_num[I-2] << float_num[I-3] <<"\n";

            ac_fixed<W, I, true> shift_float_num1;
            ac_fixed<W, I, true> comp_float_num1;
            ac_fixed<W, I, true> comp_float_num2;
            ac_fixed<W, I, true> float_num = input;


            // Check if the first 3 bits are 000
            if (  float_num.template slc<4>(W-I-3) == 0 ) {

                // Prepare first array (~f>>2)
                comp_float_num1 = float_num>>2;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Prepare second array (~f>>5)
                comp_float_num2 = float_num>>5;
                comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products  
                // Here b is 0 (zero)              
                float_num_out = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(1);

            }

            // Check if the first 3 bits are 001
            else if (  float_num.template slc<4>(W-I-3) == 1 ) {

                // Prepare first array (f>>5)
                shift_float_num1 = float_num>>5;

                // Prepare second array (~f>>2)
                comp_float_num1 = float_num>>2;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Prepare third array (~f>>7)
                comp_float_num2 = float_num>>7;
                comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products                
                float_num_out = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.9912109375);

            }
            // Check if the first 3 bits are 010
            else if ( float_num.template slc<3>(W-I-3) == 2 ) {

                // Prepare first array (~f>>3)
                comp_float_num1 = float_num>>3;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Prepare secont array (~f>>7)
                comp_float_num2 = float_num>>7;
                comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products                
                float_num_out = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.9716796875);

            }

            // Check if the first 3 bits are 011
            else if (  float_num.template slc<4>(W-I-3) == 3 ) {

                // Prepare first array (~f>>4)
                comp_float_num1 = float_num>>4;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Prepare secont array (f>>7)
                shift_float_num1 = float_num>>7;

                // Add the products                
                float_num_out = float_num + shift_float_num1 + comp_float_num1 + ac_fixed<W, I, 0>(0.94140625);

            }
            
            // Check if the first 3 bits are 011
            else if (  float_num.template slc<4>(W-I-3) == 4 ) {

                // Prepare first array (f>>5)
                shift_float_num1 = float_num>>5;

                // Prepare second array (~f>>7)
                comp_float_num1 = float_num>>7;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products                
                float_num_out = float_num + shift_float_num1 + comp_float_num1  + ac_fixed<W, I, 0>(0.90234375);

            }
            // Check if the first 3 bits are 101
            else if (  float_num.template slc<4>(W-I-3) == 5 ) {

                // Prepare first array (~f>>3)
                comp_float_num1 = float_num>>3;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Prepare second array (~f>>7)
                comp_float_num2 = float_num>>7;
                comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products                
                float_num_out = float_num + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.84375);

            }
            // Check if the first 3 bits are 110
            else if (  float_num.template slc<4>(W-I-3) == 6 ) {

                // Prepare first array (f>>2)
                shift_float_num1 = float_num>>2;

                // Prepare second array (~f>>5)
                comp_float_num1 = float_num>>5;
                comp_float_num1 = ~comp_float_num1 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Prepare third array (~f>>7)
                comp_float_num2 = float_num>>7;
                comp_float_num2 = ~comp_float_num2 +  (ac_fixed<W, I, true>(1) >> (W-I)) ;

                // Add the products                
                float_num_out = float_num + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, 0>(0.7734375);

            }
            
            // Check if the first 3 bits are 111
            else if (  float_num.template slc<4>(W-I-3) == 7 ) {

                // Here we need only shifts!

                // Prepare first array (~f>>2)
                shift_float_num1 = float_num >> 2;

                // Prepare second array (~f>>4)
                comp_float_num1 = float_num>>4;

                // Prepare third array (~f>>7)
                comp_float_num2 = float_num>>7;


                // Add the products                
                float_num_out = float_num  + shift_float_num1 + comp_float_num1 + comp_float_num2 + ac_fixed<W, I, true>(0.6787109375);
            }
            
        }

        void operator = (const float &in) {
            num = (ac_fixed<W,I,T>)(abs(in));   // The number must be positive
            sign = in>=0? 0 : 1;
        }

        void operator = (const ac_int<W,true> &in) {
            num =  (ac_fixed<W,I,T>)(in);
            num[W-1] = 0;   // The number must be positive
            sign = in[W-1];
        }
        
        void operator = (const fast_log<W,I,T> &in) {
            num = in.num;
            sign = in.sign;
        }

        void operator>>=(int shift) {
            fast_log<W, I, T> num1_log;
            fast_log<W, I, T> ans_num;
            float2log(*this,num1_log);
            num1_log.num = num1_log.num >> shift; 
            log2float(num1_log,ans_num);
            num = ans_num.num;
            }

        void operator<<=(int shift) {
            fast_log<W, I, T> num1_log;
            fast_log<W, I, T> ans_num;
            float2log(*this,num1_log);
            num1_log.num = num1_log.num << shift; 
            log2float(num1_log,ans_num);
            num = ans_num.num;
            }

        fast_log<W, I, T> operator * (const fast_log<W, I, T> &b) {
            fast_log<W, I, T> num1_log;
            fast_log<W, I, T> num2_log;
            fast_log<W, I, T> ans_log;
            fast_log<W, I, T> ans_num;

            
            float2log(*this,num1_log);
            float2log(b,num2_log);
            ans_log.num = num1_log.num + num2_log.num;
            log2float(ans_log,ans_num);
            ans_num.sign = sign ^ b.sign;
            return (ans_num);

        }

        fast_log<W, I, T> operator / (const fast_log<W, I, T> &b) {
            fast_log<W, I, T> num1_log;
            fast_log<W, I, T> num2_log;
            fast_log<W, I, T> ans_log;
            fast_log<W, I, T> ans_num;

            
            float2log(*this,num1_log);
            float2log(b,num2_log);
            ans_log.num = num1_log.num - num2_log.num;
            log2float(ans_log,ans_num);
            ans_num.sign = sign ^ b.sign;
            return (ans_num);

        }

        fast_log<W, I, T> &operator *= (const fast_log<W, I, T> &b) {
            *this = this->operator*(b);
            return *this;
        }

        bool operator == (const fast_log<W, I, T> b) {
            return ((num == b.num) && (sign == b.sign));
        }

        bool operator != (const fast_log<W, I, T> b) {
            return ((num != b.num) && (sign != b.sign));
        }

        bool operator >  (const fast_log<W, I, T> b) {
            bool num_a_greater;
            bool num_b_greater;
            bool greater;
            
            num_a_greater = num > b.num;
            num_b_greater = b.num > num;
            greater = (sign == 0) ? num_a_greater : num_b_greater;

            return ((sign < b.sign ) || ((sign == b.sign) && (greater)));
        }

        bool operator <  (const fast_log<W, I, T> b) {
            bool num_a_less;
            bool num_b_less;
            bool less;
            
            num_a_less = num < b.num;
            num_b_less = b.num > num;
            less = (sign == 0) ? num_a_less : num_b_less;

            return ((sign > b.sign ) || ((sign == b.sign) && (less)));
        }

        bool operator >= (const fast_log<W, I, T> b) {
            bool num_a_greater_eq;
            bool num_b_greater_eq;
            bool greater_eq;
            
            num_a_greater_eq = num >= b.num;
            num_b_greater_eq = b.num >= num;
            greater_eq = (sign == 0) ? num_a_greater_eq : num_b_greater_eq;

            return ((sign < b.sign ) || ((sign == b.sign) && (greater_eq)));
        }

        bool operator <= (const fast_log<W, I, T> b) {
            bool num_a_less_eq;
            bool num_b_less_eq;
            bool less_eq;
            
            num_a_less_eq = num <= b.num;
            num_b_less_eq = b.num >= num;
            less_eq = (sign == 0) ? num_a_less_eq : num_b_less_eq;

            return ((sign > b.sign ) || ((sign == b.sign) && (less_eq)));
        }
        


};



#endif
