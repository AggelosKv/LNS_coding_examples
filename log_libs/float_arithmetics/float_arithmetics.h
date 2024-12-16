#ifndef FLOAT_ARITMETICS_H
#define FLOAT_ARITMETICS_H

#include "ac_std_float.h"
#include "ac_int.h"
#include <ac_fixed.h> 



template<int W,int E>
class fast_log{
    private:

        ac_std_float<W,E> one;


        ac_int<W,false> e_bias ;    // exponent bias shifted by W-E bits

        ac_int<W,false> c_mul;

        ac_int<W,false> c_div;

        ac_int<W,false> c_sq;  // Cc-B

        ac_int<W,false> c_inv;   // Cc + 2*B

        ac_int<W,false> c_sqrt;   // 2*Cc+ B

        ac_int<W,false> c_inv_sqrt;  // 2*Cc + 3*B

        ac_int<W,false> c_div_by_sqrt;  // B + 2*Cc


        ac_fixed<W,E> mul_fault;

        ac_fixed<W,E> div_fault;

        ac_fixed<W,E> sq_fault;  

        ac_fixed<W,E> inv_fault;   

        ac_fixed<W,E> sqrt_fault;   

        ac_fixed<W,E> inv_sqrt_fault;

        ac_fixed<W,E> div_by_sqrt_fault; 

    public:

        ac_std_float<W,E> num;
        

        // Constructors
        fast_log() {
            one = ac_std_float<W,E>(1);

            e_bias = one.data_ac_int();

            mul_fault         = ac_fixed<W,E>(0.0545458); 
            div_fault         = ac_fixed<W,E>(-0.0593111); 
            sq_fault          = ac_fixed<W,E>(0.05409255339);   
            inv_fault         = ac_fixed<W,E>(-0.1171358);    
            sqrt_fault        = ac_fixed<W,E>(-0.0558151667);    
            inv_sqrt_fault    = ac_fixed<W,E>(-0.1715728752538); 
            div_by_sqrt_fault = ac_fixed<W,E>(-0.059443);  
            
            for (int i=0; i<W; i++){
                c_mul[i]         = mul_fault[i]; 
                c_div[i]         = div_fault[i]; 
                c_sq[i]          = sq_fault[i]; 
                c_inv[i]         = inv_fault[i]; 
                c_sqrt[i]        = sqrt_fault[i]; 
                c_inv_sqrt[i]    = inv_sqrt_fault[i]; 
                c_div_by_sqrt[i] = div_by_sqrt_fault[i];   
            }

            c_mul         -= e_bias;
            c_div         +=  e_bias;
            c_sq          -= e_bias;
            c_inv         += (e_bias<<1);
            c_sqrt        += e_bias;
            c_inv_sqrt    += (e_bias<<1) + e_bias;
            c_div_by_sqrt += e_bias;
        };

        // Constactor from float
        fast_log(const float &in) {
            this->operator=(in);
        }

        // Constractor from AC_INT
        fast_log(const ac_int<W,false> &in) {
            this->operator=(in);
        }
        // Copy constactor
        fast_log(const fast_log<W,E> &in) {
            this->operator=(in);
        }
        // De-constactor
        ~fast_log() {};

        void set(ac_std_float<W,E> num_s) {
            num = num_s;
        }



        void print() const {
            std::cout << "fast_log(" << num << ")" << std::endl;
        }



        void operator = (const float &in) {
            num = (ac_std_float<W,E>)(in);   
        }

        void operator = (const ac_int<W,true> &in) {
            ac_int<W,true> temp = in;
            num.set_data(temp);
        }
        
        void operator = (const fast_log<W,E> &in) {
            num = in.num;
        }


        fast_log<W, E> operator + (const fast_log<W, E> &b) {
            fast_log<W, E> ans_num;
                    ans_num.num = num + b.num;
            return (ans_num);

        }

            fast_log<W, E> operator - (const fast_log<W, E> &b) {
            fast_log<W, E> ans_num;
                    ans_num.num = num - b.num;
            return (ans_num);

        }

        fast_log<W, E> operator * (const fast_log<W, E> &b) {
            ac_int<W,false> log_ans;
            fast_log<W, E> ans;

            log_ans = num.data_ac_int() + b.num.data_ac_int() + c_mul ;

            ans.num.set_data(log_ans);

            return (ans);

        }

        fast_log<W, E> operator / (const fast_log<W, E> &b) {
            ac_int<W,false> log_ans;
            fast_log<W, E> ans;

            log_ans = num.data_ac_int() - b.num.data_ac_int() + c_div ;
            ans.num.set_data(log_ans);

            return (ans);

        }

        void operator>>=(int shift) {
            num.set_data(((num.data_ac_int() + c_sqrt )>> shift)); 
        }

        void operator<<=(int shift) {
            num.set_data((num.data_ac_int() << shift) + c_sq); 
        }


        void sqrt() {
            num.set_data(((num.data_ac_int() + c_sqrt )>> 1)); 
        }
		// squert (A^2)
        void sq() {
            num.set_data((num.data_ac_int() << 1) + c_sq); 
        }


        // inverted number (1/x) (reciprocal)
        void inv() {
            num.set_data(c_inv - num.data_ac_int() );
        }
        
        // 1/ sqrt(x)
        void inv_sqrt() {
            num.set_data((c_inv_sqrt - num.data_ac_int() ) >> 1);
        }

        // x/ sqrt(y)
        void div_by_sqrt(fast_log<W, E> b) {
            ac_int<W+1,E+1> temp;

            temp = (num.data_ac_int() + ((c_div_by_sqrt - b.num.data_ac_int())>> 1)  );
            temp[W-1] = temp[W];

            num.set_data( temp.template slc<W>(0));
        }



        fast_log<W, E> &operator *= (const fast_log<W, E> &b) {
            *this = this->operator*(b);
            return *this;
        }

        fast_log<W, E> &operator += (const fast_log<W, E> &b) {
            *this = this->operator+(b);
            return *this;
        }

        bool operator == (const fast_log<W, E> b) {
            return ((num == b.num));
        }

        bool operator != (const fast_log<W, E> b) {
            return ((num != b.num));
        }

        bool operator >  (const fast_log<W, E> b) {

            ac_int<W,false> num_a = num.data_ac_int();
            ac_int<W,false> num_b = b.num.data_ac_int();

            ac_int<1,false> sign_a = num_a[W-1];
            ac_int<1,false> sign_b = num_b[W-1];
            num_a[W-1] = 0;
            num_b[W-1] = 0;


            bool num_a_greater;
            bool num_b_greater;
            bool greater;
            
            num_a_greater = num_a > num_b;
            num_b_greater = num_b > num_a;
            greater = (sign_a == 0) ? num_a_greater : num_b_greater;

            return ((sign_a < sign_b ) || ((sign_a == sign_b) && (greater)));
        }

        bool operator <  (const fast_log<W, E> b) {

            ac_int<W,false> num_a = num.data_ac_int();
            ac_int<W,false> num_b = b.num.data_ac_int();

            ac_int<1,false> sign_a = num_a[W-1];
            ac_int<1,false> sign_b = num_b[W-1];
            num_a[W-1] = 0;
            num_b[W-1] = 0;

            bool num_a_less;
            bool num_b_less;
            bool less;
            
            num_a_less = num_a < num_b;
            num_b_less = num_b > num_a;
            less = (sign_a == 0) ? num_a_less : num_b_less;

            return ((sign_a > num_b ) || ((sign_a == num_b) && (less)));
        }

        bool operator >= (const fast_log<W, E> b) {
            ac_int<W,false> num_a = num.data_ac_int();
            ac_int<W,false> num_b = b.num.data_ac_int();

            ac_int<1,false> sign_a = num_a[W-1];
            ac_int<1,false> sign_b = num_b[W-1];
            num_a[W-1] = 0;
            num_b[W-1] = 0;

            bool num_a_greater_eq;
            bool num_b_greater_eq;
            bool greater_eq;
            
            num_a_greater_eq = num_a >= num_b;
            num_b_greater_eq = num_b >= num_a;
            greater_eq = (sign_a == 0) ? num_a_greater_eq : num_b_greater_eq;

            return ((sign_a < sign_b ) || ((sign_a == sign_b) && (greater_eq)));
        }

        bool operator <= (const fast_log<W, E> b) {

            ac_int<W,false> num_a = num.data_ac_int();
            ac_int<W,false> num_b = b.num.data_ac_int();

            ac_int<1,false> sign_a = num_a[W-1];
            ac_int<1,false> sign_b = num_b[W-1];
            num_a[W-1] = 0;
            num_b[W-1] = 0;

            bool num_a_less_eq;
            bool num_b_less_eq;
            bool less_eq;
            
            num_a_less_eq = num_a <= num_b;
            num_b_less_eq = num_b >= num_a;
            less_eq = (sign_a == 0) ? num_a_less_eq : num_b_less_eq;

            return ((sign_a > sign_b ) || ((sign_a == sign_b) && (less_eq)));
        }
        
        template<int N>
        void dotProd(fast_log<W, E> A[N],fast_log<W, E> B[N]) {

            fast_log<W, E> sum =  fast_log<W, E>(0) ;

            for (int i=0; i<N; i++){
                sum += A[i] * B[i];
            }

            num = sum.num;
        }

        void mac(fast_log<W, E> A,fast_log<W, E> B,fast_log<W, E> C) {

            fast_log<W, E> sum =  fast_log<W, E>(0) ;

                sum = A * B + C;

            num = sum.num;
        }

        void copy_value(ac_fixed<W,E> value , ac_int<W,false> destination){
            for (int i=0; i<W; i ++){
                destination[i] = value[i];
            }
        }


};



#endif
