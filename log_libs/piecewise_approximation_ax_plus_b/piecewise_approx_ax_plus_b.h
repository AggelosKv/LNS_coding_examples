#ifndef PIECEWICE_APPROX_AX_PLUS_B_H
#define PIECEWICE_APPROX_AX_PLUS_B_H

#include "ac_fixed.h"


template<int W,int I>
class fast_log{
    public:

        ac_fixed<W,I,true>  num;
        ac_int<1,false> sign;    // 1'b0 positive, 1'b1 negative

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
        fast_log(const fast_log<W,I> &in) {
            this->operator=(in);
        }
        // De-constactor
        ~fast_log() {};

        void set(ac_int<1,false> sign_s, ac_fixed<W,I,true> num_s) {
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
            

            for (int i = (W - I) ; i <= W-1; i++) {
                integerPart += num[i] * base; // binaryNum[i] * (2^(frac_start-1-i))
                base *= 2; // Update base for the next bit
            }

            // Calculate the fractional part
            float fractionalPart = 0.0;
            base = 0.5; // This will represent 2^(-1), 2^(-2), ...
            
            for (int i = W - I-1; i > 0; i--) {
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
        void float2log(const fast_log<W, I> float_num , fast_log<W, I> &float_num_out1){

            bool one_found = false;
            int i;
            int one_place;
            
            ac_fixed<W, I, true> temp_num = float_num.num;
            ac_fixed<W, I, true> float_num_temp_out;

            // Find the leading one in the integer part
            for (i = W-1; i > 0; i--){
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
           // std::cout << "The float part first three bit were : " << input.template slc<4>(W-I-3) <<"\n";

            ac_fixed<W, I, true> shift_float_num1;
            ac_fixed<W, I, true> comp_float_num1;
            ac_fixed<W, I, true> comp_float_num2;
            ac_fixed<W, I, true> float_num = input;
            // Check if the first 3 bits are 000
            if (  float_num.template slc<4>(W-I-3) == 0 ) {
      
                float_num = alpha_log_lut[0] *float_num;

            }
            
            // Check if the first 3 bits are 001
            else if (  float_num.template slc<4>(W-I-3) == 1 ) {

                float_num = alpha_log_lut[1] *float_num + beta_log_lut[1];


            }
            // Check if the first 3 bits are 010
            else if (  float_num.template slc<4>(W-I-3) == 2 ) {

                float_num = alpha_log_lut[2] *float_num + beta_log_lut[2];


            }
            // Check if the first 3 bits are 011
            else if (  float_num.template slc<4>(W-I-3) == 3 ) {

                float_num = alpha_log_lut[3] *float_num + beta_log_lut[3];


            }
            // Check if the first 3 bits are 100
            else if (  float_num.template slc<4>(W-I-3) == 4 ) {

                float_num = alpha_log_lut[4] *float_num + beta_log_lut[4];


            }
            // Check if the first 3 bits are 101
            else if (  float_num.template slc<4>(W-I-3) == 5 ) {

                float_num = alpha_log_lut[5] *float_num + beta_log_lut[5];


            }
            // Check if the first 3 bits are 110
            else if (  float_num.template slc<4>(W-I-3) == 6 ) {

                float_num = alpha_log_lut[6] *float_num + beta_log_lut[6];


            }
            // Check if the first 3 bits are 111
            else if (  float_num.template slc<4>(W-I-3) == 7 ) {

                float_num = alpha_log_lut[7] *float_num + beta_log_lut[7];

            }
            //std::cout << "The float num after the LUT is : " << float_num <<"\n";
            float_num_out = float_num;
        }

        // We call this function to make the conversion from a logarithm to a float number
        void log2float(const fast_log<W, I> input, fast_log<W, I> &float_num_out ){
            int integer;
            ac_int< I, false> temp;
            ac_fixed<W, I, true> float_num_temp_out;
            temp = 0;
            ac_fixed<W, I, true> float_num = input.num;

            // store the integer
            integer=float_num.to_int();

            // clear the integer part
            float_num.set_slc(W-I, temp);  // Set the integer part to zero

            // run the LUT
            //std::cout << "Before LUT " << float_num << "\n";
            // Run the peicewice approximation ( The integer part must me missing)
            LUT_log2float(float_num, float_num_temp_out);
           // std::cout << "after LUT " << float_num_temp_out << "\n";

            // Shift the binary num Left integer times
            float_num_temp_out <<= integer;
            
            float_num_out.num = float_num_temp_out;

            
        }

        // This function performs the piecewise aproximation to convert the fractional part of a logarithm to its float aproximate
        // The input range is only the fraction part 0 <= f < 1 and it is divided in 8 equal spaces
        void LUT_log2float(const ac_fixed<W, I, true> input, ac_fixed<W, I, true> &float_num_out){

           // std::cout << "The float part first three bit were : " << input.template slc<4>(W-I-3) <<"\n";

            ac_fixed<W, I, true> shift_float_num1;
            ac_fixed<W, I, true> comp_float_num1;
            ac_fixed<W, I, true> comp_float_num2;
            ac_fixed<W, I, true> float_num = input;


            // Check if the first 3 bits are 000
            if (  float_num.template slc<4>(W-I-3) == 0 ) {

                float_num_out = alpha_antilog_lut[0] *float_num + beta_antilog_lut[0];

            }

            // Check if the first 3 bits are 001
            else if (  float_num.template slc<4>(W-I-3) == 1 ) {

                float_num_out = alpha_antilog_lut[1] *float_num + beta_antilog_lut[1];

            }
            // Check if the first 3 bits are 010
            else if ( float_num.template slc<3>(W-I-3) == 2 ) {

                float_num_out = alpha_antilog_lut[2] *float_num + beta_antilog_lut[2];

            }

            // Check if the first 3 bits are 011
            else if (  float_num.template slc<4>(W-I-3) == 3 ) {

                float_num_out = alpha_antilog_lut[3] *float_num + beta_antilog_lut[3];

            }
            
            // Check if the first 3 bits are 011
            else if (  float_num.template slc<4>(W-I-3) == 4 ) {

                float_num_out = alpha_antilog_lut[4] *float_num + beta_antilog_lut[4];

            }
            // Check if the first 3 bits are 101
            else if (  float_num.template slc<4>(W-I-3) == 5 ) {

                float_num_out = alpha_antilog_lut[5] *float_num + beta_antilog_lut[5];

            }
            // Check if the first 3 bits are 110
            else if (  float_num.template slc<4>(W-I-3) == 6 ) {

                float_num_out = alpha_antilog_lut[6] *float_num + beta_antilog_lut[6];

            }
            
            // Check if the first 3 bits are 111
            else if (  float_num.template slc<4>(W-I-3) == 7 ) {

                float_num_out = alpha_antilog_lut[7] *float_num + beta_antilog_lut[7];

            }
            
        }

        void operator = (const float &in) {
            num = (ac_fixed<W,I,true>)(in>0? in : -in);   // The number must be positive
            sign = in>=0? 0 : 1;
        }

        void operator = (const ac_int<W,true> &in) {
            num =  (ac_fixed<W,I,true>)(in);
            num[W-1] = 0;   // The number must be positive
            sign = in[W-1];
        }
        
        void operator = (const fast_log<W,I> &in) {
            num = in.num;
            sign = in.sign;
        }

        void operator>>=(int shift) {
            fast_log<W, I> num1_log;
            fast_log<W, I> ans_num;
            float2log(*this,num1_log);
            num1_log.num = num1_log.num >> shift; 
            log2float(num1_log,ans_num);
            num = ans_num.num;
            }

        void operator<<=(int shift) {
            fast_log<W, I> num1_log;
            fast_log<W, I> ans_num;
            float2log(*this,num1_log);
            num1_log.num = num1_log.num << shift; 
            log2float(num1_log,ans_num);
            num = ans_num.num;
            }

        fast_log<W, I> operator + (const fast_log<W, I> &b) {
            fast_log<W, I> ans_num;
            if (!(sign ^ b.sign)){
                ans_num.num = num + b.num;
                ans_num.sign = sign;
            }
            else if (num > b.num){
                ans_num.num = num - b.num;
                ans_num.sign = sign;
            }
            else{
                ans_num.num = b.num - num;
                ans_num.sign = b.sign;
            }
             ans_num.num[W-1] = 0;
            return (ans_num);

        }
        
        fast_log<W, I> operator - (const fast_log<W, I> &b) {
            fast_log<W, I> ans_num;
            if (!(sign ^ b.sign) || (b.sign)){
                ans_num.num = num + b.num;
                ans_num.sign = sign;
            }
            else if (num > b.num){
                ans_num.num = num - b.num;
                ans_num.sign = sign;
            }
            else{
                ans_num.num = b.num - num;
                ans_num.sign = b.sign;
            }
             ans_num.num[W-1] = 0;
            return (ans_num);

        }

        fast_log<W, I> operator * (const fast_log<W, I> &b) {
            fast_log<W, I> num1_log;
            fast_log<W, I> num2_log;
            fast_log<W, I> ans_log;
            fast_log<W, I> ans_num;

            
            float2log(*this,num1_log);
            float2log(b,num2_log);
            ans_log.num = num1_log.num + num2_log.num;
            log2float(ans_log,ans_num);
            ans_num.sign = sign ^ b.sign;
            return (ans_num);

        }

        fast_log<W, I> operator / (const fast_log<W, I> &b) {
            fast_log<W, I> num1_log;
            fast_log<W, I> num2_log;
            fast_log<W, I> ans_log;
            fast_log<W, I> ans_num;

            
            float2log(*this,num1_log);
            float2log(b,num2_log);
            ans_log.num = num1_log.num - num2_log.num;
            log2float(ans_log,ans_num);
            ans_num.sign = sign ^ b.sign;
            return (ans_num);

        }

        
        // power of number (A^(2^n))
        void pow (const ac_int<W,false> root) {
            fast_log<W, I> num1_log;
            fast_log<W, I> ans_log;
            fast_log<W, I> ans_num;

                if (root == 1){
                    ans_log.num = 1;
                }
                else{
                    float2log(*this,num1_log);
                    ans_log.num = num1_log.num << root;


                    if ((root %2) == 0){
                        ans_num.sign = sign;
                    }else{
                        ans_num.sign = 0;
                    }
                    
                    log2float(ans_log,ans_num);
                    num = ans_num.num;
                    sign = ans_num.sign;
                }
        }


        // squeare root (sqrt(A))
        void sqrt () {
            fast_log<W, I> num1_log;
            fast_log<W, I> ans_log;
            fast_log<W, I> ans_num;

            
            float2log(*this,num1_log);

            ans_log.num = num1_log.num >> 1;

            log2float(ans_log,ans_num);
            num = ans_num.num;
            sign = 0;

        }

        // squeare (A^2)
        void sq () {
            fast_log<W, I> num1_log;
            fast_log<W, I> ans_log;
            fast_log<W, I> ans_num;

            
            float2log(*this,num1_log);

            ans_log.num = num1_log.num << 1;

            log2float(ans_log,ans_num);
            num = ans_num.num;
            sign = 0;

        }

        fast_log<W, I> &operator *= (const fast_log<W, I> &b) {
            *this = this->operator*(b);
            return *this;
        }

        fast_log<W, I> &operator += (const  fast_log<W, I> &b) {
            *this = this->operator+(b);
            return *this;
        }

        bool operator == (const fast_log<W, I> b) {
            return ((num == b.num) && (sign == b.sign));
        }

        bool operator != (const fast_log<W, I> b) {
            return ((num != b.num) && (sign != b.sign));
        }

        bool operator >  (const fast_log<W, I> b) {
            bool num_a_greater;
            bool num_b_greater;
            bool greater;
            
            num_a_greater = num > b.num;
            num_b_greater = b.num > num;
            greater = (sign == 0) ? num_a_greater : num_b_greater;

            return ((sign < b.sign ) || ((sign == b.sign) && (greater)));
        }

        bool operator <  (const fast_log<W, I> b) {
            bool num_a_less;
            bool num_b_less;
            bool less;
            
            num_a_less = num < b.num;
            num_b_less = b.num > num;
            less = (sign == 0) ? num_a_less : num_b_less;

            return ((sign > b.sign ) || ((sign == b.sign) && (less)));
        }

        bool operator >= (const fast_log<W, I> b) {
            bool num_a_greater_eq;
            bool num_b_greater_eq;
            bool greater_eq;
            
            num_a_greater_eq = num >= b.num;
            num_b_greater_eq = b.num >= num;
            greater_eq = (sign == 0) ? num_a_greater_eq : num_b_greater_eq;

            return ((sign < b.sign ) || ((sign == b.sign) && (greater_eq)));
        }

        bool operator <= (const fast_log<W, I> b) {
            bool num_a_less_eq;
            bool num_b_less_eq;
            bool less_eq;
            
            num_a_less_eq = num <= b.num;
            num_b_less_eq = b.num >= num;
            less_eq = (sign == 0) ? num_a_less_eq : num_b_less_eq;

            return ((sign > b.sign ) || ((sign == b.sign) && (less_eq)));
        }
        
        template<int N>
        void dotProd(fast_log<W, I> A[N],fast_log<W, I> B[N]) {

            fast_log<W, I> sum =  fast_log<W, I>(0) ;

            for (int i=0; i<N; i++){
                sum += A[i] * B[i];
            }

            num = sum.num;
            sign = sum.sign;
        }

        void mac(fast_log<W, I> A,fast_log<W, I> B,fast_log<W, I> C) {

            fast_log<W, I> sum =  fast_log<W, I>(0) ;

                sum = A * B + C;

            num = sum.num;
        }

    const ac_fixed<W-I+1, 1, 0>
    alpha_log_lut[8] = {
        175.0f/128.0f,
        155.0f/128.0f,
        142.0f/128.0f,
        129.0f/128.0f,
        119.0f/128.0f,
        110.0f/128.0f,
        102.0f/128.0f,
        95.0f/128.0f
    };

    const ac_fixed<W-I+1, 1, 0> 
    beta_log_lut[8] = {
        0.0f,
        20.0f/1024.0f,
        46.0f/1024.0f,
        84.0f/1024.0f,
        123.0f/1024.0f,
        167.0f/1024.0f,
        215.0f/1024.0f,
        264.0f/1024.0f
    };

    const ac_fixed<W-I+1, 1, 0>
    alpha_antilog_lut[8] = {
        92.0f/128.0f,
        93.0f/128.0f,
        111.0f/128.0f,
        121.0f/128.0f,
        132.0f/128.0f,
        143.0f/128.0f,
        155.0f/128.0f,
        169.0f/128.0f
    };

    const ac_fixed<W-I+1, 1, 0> 
    beta_antilog_lut[8] = {
        1024.0f/1024.0f,
        1015.0f/1024.0f,
        995.0f/1024.0f,
        964.0f/1024.0f,
        924.0f/1024.0f,
        864.0f/1024.0f,
        792.0f/1024.0f,
        295.0f/1024.0f
    };


};



#endif
