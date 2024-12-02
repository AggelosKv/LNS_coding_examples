#ifndef FLOAT_AND_LOG_OP_H
#define FLOAT_AND_LOG_OP_H

#include "ac_std_float.h"
#include "ac_int.h"
#include <ac_fixed.h> 



template<int W,int E>
class fast_log{
    public:

        typedef ac_std_float<W,E> num_t;
        
        num_t  num;
        
        const ac_int<W,false> e_bias = num_t(1).data_ac_int();    // exponent bias shifted by W-E bits

        const ac_int<W,false> c_mul = ((ac_fixed<W,E>(0.0545458)).template slc<W>(0)) - e_bias;

        const ac_int<W,false> c_div = ((ac_fixed<W,E>(-0.0593111)).template slc<W>(0)) + e_bias;

        const ac_int<W,false> c_sq = ((ac_fixed<W,E>(0.05409255339)).template slc<W>(0)) - e_bias;    // Cc-B

        const ac_int<W,false> c_inv = ((ac_fixed<W,E>(-0.1171358)).template slc<W>(0)) + (e_bias<<1);  // Cc + 2*B

        const ac_int<W,false> c_sqrt = ((ac_fixed<W,E>(-0.05581516662433070558320861061912)).template slc<W>(0)) + e_bias;   // 2*Cc+ B

        const ac_int<W+1,false> c_inv_sqrt = ((ac_fixed<W,E>(-0.1715728752538099023966225515806)).template slc<W>(0)) + (e_bias<<1) + e_bias;  // 2*Cc + 3*B

        const ac_int<W,false> c_div_by_sqrt = ((ac_fixed<W,E>(-0.059443)).template slc<W>(0)) + e_bias;     // B + 2*Cc


        // Constructors
        fast_log() {};

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

        void set(num_t num_s) {
            num = num_s;
        }



        void print() const {
            std::cout << "fast_log(" << num << ")" << std::endl;
        }



        void operator = (const float &in) {
            num = (ac_std_float<W,E>)(in);   // The number must be positive
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


        template<int N>
        void dotProd(fast_log<W, E> A[N],fast_log<W, E> B[N]) {

            fast_log<W, E> sum_array[N];
            fast_log<W, E> sum =  fast_log<W, E>(0) ;

            for (int i=0; i<N; i++){
                sum_array[i] = A[i] * B[i];
            }
            for(int i=0; i<N; i++){
                sum =  sum_array[i] + sum;
            }

            num = sum.num;
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
        

};



#endif
