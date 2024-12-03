#ifndef FLOAT_AND_LOG_OP_H
#define FLOAT_AND_LOG_OP_H

#include "ac_fixed.h"


template<int W,int I,bool T>
class fast_log{
    public:

        ac_fixed<W,I,T>  num;
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
        fast_log(const fast_log<W,I,T> &in) {
            this->operator=(in);
        }
        // De-constactor
        ~fast_log() {};

        void set(ac_int<1,false> sign_s, ac_fixed<W,I,T> num_s) {
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

            // Insert the new integer part
            if(one_found == true){
                float_num_out1.num = temp_num + ac_fixed<W, I, true>(one_place);
            }
            else{
                float_num_out1.num = temp_num;
            }
        }



        // We call this function to make the conversion from a logarithm to a float number
        void log2float(const fast_log<W, I, T> input, fast_log<W, I, T> &float_num_out ){
            int integer;
            ac_int< W - I, true> temp;
            temp = 0;
            ac_fixed<W, I, true> float_num = input.num;

            // store the integer
            integer=float_num.to_int();

            // clear the integer part
            float_num.set_slc(I, temp);  // Set the integer part to zero
            float_num[I] = 1;

            float_num <<= integer;
            
            float_num_out.num = float_num;

        }

        void operator = (const float &in) {
            num = (ac_fixed<W,I,T>)(in>0? in : -in);   // The number must be positive
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

        fast_log<W, I, T> operator + (const fast_log<W, I, T> &b) {
            fast_log<W, I, T> ans_num;
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
        
        fast_log<W, I, T> operator - (const fast_log<W, I, T> &b) {
            fast_log<W, I, T> ans_num;
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

        fast_log<W, I, T> &operator += (const  fast_log<W, I, T> &b) {
            *this = this->operator+(b);
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
        
        template<int N>
        void dotProd(fast_log<W, I, T> A[N],fast_log<W, I, T> B[N]) {

            fast_log<W, I, T> sum =  fast_log<W, I, T>(0) ;

            for (int i=0; i<N; i++){
                sum += A[i] * B[i];
            }

            num = sum.num;
            sign = sum.sign;
        }

        void mac(fast_log<W, I, T> A,fast_log<W, I, T> B,fast_log<W, I, T> C) {

            fast_log<W, I, T> sum =  fast_log<W, I, T>(0) ;

                sum = A * B + C;

            num = sum.num;
        }
};



#endif
