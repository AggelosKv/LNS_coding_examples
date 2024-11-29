#ifndef FLOAT_AND_LOG_OP_H
#define FLOAT_AND_LOG_OP_H

#include "ac_int.h"


template<int W,bool T>
class iter_mul_num{
    public:

        typedef ac_int<W,false> num_t;
        typedef ac_int<1,false> sign_t;

        num_t  num;
        sign_t sign;    // 1'b0 positive, 1'b1 negative

        // Constructors
        iter_mul_num() {};

        // Constactor from float
        iter_mul_num(const float &in) {
            this->operator=(in);
        }

        // Constractor from AC_INT
        iter_mul_num(const ac_int<W,true> &in) {
            this->operator=(in);
        }
        // Copy constactor
        iter_mul_num(const iter_mul_num<W,T> &in) {
            this->operator=(in);
        }
        // De-constactor
        ~iter_mul_num() {};

        void set(sign_t sign_s, num_t num_s) {
            sign = sign_s;
            num = num_s;
        }



        float to_float() {
            
            if (W <= 0) {
                return (float(0)); // Invalid input; set to 0
            }

            // Calculate the integer part
            float integerPart = 0.0;
            float base = 1.0; // This will represent 2^0, 2^1, 2^2, ...
            float final_num;
            

            for (int i = 0 ; i <= W-1; i++) {
                integerPart += num[i] * base; // binaryNum[i] * (2^(frac_start-1-i))
                base *= 2; // Update base for the next bit
            }


            // Combine both parts
            final_num = integerPart ;
        
            if (sign == 1 ){
                final_num = final_num * -1;
            }

            return  final_num;
        }


        void print() const {
            std::cout << "iter_mul_num(" << num << ")" << std::endl;
        }


    // Funtion to convert binary to log and mul using iterations
    void iter_mul(const iter_mul_num<W, T> in_num1, const iter_mul_num<W, T> in_num2 , iter_mul_num<W, T> &num_out) {
        ac_int<W,false> temp_num1 = ac_int<W,false>(0); // Initialize with zero 
        ac_int<W,false> temp_num2 = ac_int<W,false>(0); // Initialize with zero 
        ac_int<W,false> temp_num3 = ac_int<W,false>(1); // Initialize with zero 

        ac_int<W,false> temp1 = ac_int<W,false>(0); // Initialize with zero 
        ac_int<W,false> temp2 = ac_int<W,false>(0); // Initialize with zero 

        ac_int<W,false> place_of_one_1 = ac_int<W,false>(1);
        ac_int<W,false> place_of_one_2 = ac_int<W,false>(1);
        
        temp_num1 = in_num1.num;
        temp_num2 = in_num2.num;

        num_out = iter_mul_num<W, T>(0);

        Iterative_MUL : for (int i = 0; i<18; i++){

                            if( (place_of_one_1 == 0) || ( place_of_one_2 == 0) ){
                                std::cout << " The aproximation fount the value in "<< i <<" iterations!" << "\n";
                                break;
                            }


                            changeFirstOneToZero(temp_num1,temp1,place_of_one_1);      // calculate N-2^k1 
                            changeFirstOneToZero(temp_num2,temp2,place_of_one_2);      // calculate N-2^k2

                            temp_num1 = temp1; // N-2^k1
                            temp_num2 = temp2; // N-2^k2

                            temp1 <<= (place_of_one_2);    // calculate (N-2^k1)2^k2
                            temp2 <<= (place_of_one_1);    // calculate (N-2^k2)2^k1

                            temp_num3 <<= (place_of_one_2 + place_of_one_1); // 2^(k1+k2)
                            
                            num_out.num = num_out.num + temp_num3 + ac_int<W, false>(temp2 + temp1);   // add to the aproximation
                        
                            temp_num3 =  ac_int<W, false>(1); // clear for the next iteration


                }
    }
    
    void changeFirstOneToZero(const ac_int<W, false> num_in, ac_int<W, false> &num_out, ac_int<W,false> &place_of_one){
        ac_int<W, false> temp = num_in;
        bool found = false;

        num_out = ac_int<W, false> (0);
        place_of_one = ac_int<W,false>(0);

        for (int i = W - 1; i>= 0; i--){
            if(found == true){
                continue;
            }
            else if((temp[i] == 1) && (found == false)){
                temp[i] = 0; 
                found = true;
                place_of_one = ac_int<W,false>(i);
            }

            num_out = temp;

        }



    }
        

        void operator = (const float &in) {
            num = (ac_int<W,false>)(abs(in));   // The number must be positive
            sign = in>=0? 0 : 1;
        }

        void operator = (const ac_int<W,true> &in) {
            num =  (ac_int<W,false>)(in);
            num[W-1] = 0;   // The number must be positive
            sign = in[W-1];
        }
        
        void operator = (const iter_mul_num<W,T> &in) {
            num = in.num;
            sign = in.sign;
        }

        void operator>>=(int shift) {
            num = num >> shift; 
            }

        void operator<<=(int shift) {
            num = num << shift; 
            }

        iter_mul_num<W, T> operator + (const iter_mul_num<W, T> &b) {
            iter_mul_num<W, T> ans_num;
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

            iter_mul_num<W, T> operator - (const iter_mul_num<W, T> &b) {
            iter_mul_num<W, T> ans_num;
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

        iter_mul_num<W, T> operator * (const iter_mul_num<W, T> &b) {
                iter_mul_num<W, T> ans_num;
                iter_mul(*this, b, ans_num);
                ans_num.sign = sign ^ b.sign;
            return (ans_num);

        }

        iter_mul_num<W, T> &operator *= (const iter_mul_num<W, T> &b) {
            *this = this->operator*(b);
            return *this;
        }


        bool operator == (const iter_mul_num<W, T> b) {
            return ((num == b.num) && (sign == b.sign));
        }

        bool operator != (const iter_mul_num<W, T> b) {
            return ((num != b.num) && (sign != b.sign));
        }


        bool operator >  (const iter_mul_num<W, T> b) {
            bool num_a_greater;
            bool num_b_greater;
            bool greater;
            
            num_a_greater = num > b.num;
            num_b_greater = b.num > num;
            greater = (sign == 0) ? num_a_greater : num_b_greater;

            return ((sign < b.sign ) || ((sign == b.sign) && (greater)));
        }

        bool operator <  (const iter_mul_num<W, T> b) {
            bool num_a_less;
            bool num_b_less;
            bool less;
            
            num_a_less = num < b.num;
            num_b_less = b.num > num;
            less = (sign == 0) ? num_a_less : num_b_less;

            return ((sign > b.sign ) || ((sign == b.sign) && (less)));
        }

        bool operator >= (const iter_mul_num<W, T> b) {
            bool num_a_greater_eq;
            bool num_b_greater_eq;
            bool greater_eq;
            
            num_a_greater_eq = num >= b.num;
            num_b_greater_eq = b.num >= num;
            greater_eq = (sign == 0) ? num_a_greater_eq : num_b_greater_eq;

            return ((sign < b.sign ) || ((sign == b.sign) && (greater_eq)));
        }

        bool operator <= (const iter_mul_num<W, T> b) {
            bool num_a_less_eq;
            bool num_b_less_eq;
            bool less_eq;
            
            num_a_less_eq = num <= b.num;
            num_b_less_eq = b.num >= num;
            less_eq = (sign == 0) ? num_a_less_eq : num_b_less_eq;

            return ((sign > b.sign ) || ((sign == b.sign) && (less_eq)));
        }
        
        template<int N>
        void dotProd(iter_mul_num<W, T> A[N],iter_mul_num<W, T> B[N]) {

            iter_mul_num<W, T> sum_array[N];
            iter_mul_num<W, T> sum =  iter_mul_num<W, T>(0) ;

            for (int i=0; i<N; i++){
                sum_array[i] = A[i] * B[i];
            }
            for(int i=0; i<N; i++){
                sum =  sum_array[i] + sum;
            }

            num = sum.num;
            sign = sum.sign;
        }
        

};



#endif
