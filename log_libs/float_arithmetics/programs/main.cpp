#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_std_float.h"
#include "float_and_log_op.h"

#include <random>

#include "mc_scverify.h"

#define W 32 // Bit width for ac_fixed
#define E 8  // Integer bits for ac_fixed
#define I W-E // 
#define N 2  


float generateRandomFloat(float maxValue = 20000.0) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<float> dist(-maxValue, maxValue);
    
    return dist(gen);
}


int main(int argc, char* argv[]) {

    //Create fast_log instances
    fast_log<W, E> fast_log_num1(0);
    fast_log<W, E> fast_log_num2(0);
    fast_log<W, E> fast_log_ans(0);
    float float_num;


    int max_iterations = 1000;

    float num1;
    float num2;
    float correct;
    float error;

    bool error_flag = false;



    for (int k = 0; k < max_iterations; k++){
        // Generate and print a random floating-point number below 20000
    num1 = generateRandomFloat();
    num2 = abs(generateRandomFloat());

     

    std::cout << "Random floating-point number num1: " << num1 << std::endl;
    std::cout << "Random floating-point number num2: " << num2 << std::endl;


    fast_log_num1 = num1;
    fast_log_num2 = num2;

     //fast_log_num1.div_by_sqrt(fast_log_num2);// / fast_log_num2;
     fast_log_ans = fast_log_num1 * fast_log_num2;
     
    correct = num1 * num2;
    error = 0.246*correct;

    std::cout << "The correct result is :" << correct << "\n";
    std::cout << "The given result is :" << fast_log_ans.num.to_float() << "\n";

    correct = correct - fast_log_ans.num.to_float();    // calculate the error
    std::cout << "The error between them is :" << correct << "\n";
    std::cout << std::endl;

        if (abs(correct) > abs(error)){
            std::cout << "The error should be less than :" << error <<" \n";
            std::cout << "[ERROR]\n";
            error_flag = true;
            break;
        }
    }

    if (!error_flag){
        std::cout << "No errorus results where detected! \n";
    }


// ac_std_float<W,E> test1 = ac_std_float<W,E>(-1662.31);
// ac_std_float<W,E> test2 = ac_std_float<W,E>( 96.5042);
// ac_std_float<W,E> exponend_bias = ac_std_float<W,E>(1);


// ac_int<W,false> ac_error = ac_int<W,false>((ac_fixed<W,E>(0.0545458)).template slc<W>(0));

// fast_log<W, E> fast_log_num1(0);


//  std::cout << "The ans binary is : \n";
// for (int i = W-1; i>=0; i--){

//     std::cout<<fast_log_num1.c_div_by_sqrt[i];

// }
// std::cout<<"\n";



// ac_int<W,false> ans ;
// ac_int<W,false> int_exponend_bias;

// ac_std_float<W,E> test_fin;

// int_exponend_bias = exponend_bias.data_ac_int();
//  std::cout << "The without_exponent_bias binary is : \n";
// for (int i = W-1; i>=0; i--){

//     std::cout<<int_exponend_bias[i];

// }
// std::cout<<"\n";

//             std::cout << "test1.data_ac_int() : "<<test1.data_ac_int() << " \n";            
//                         std::cout << "test2.data_ac_int() : "<<test2.data_ac_int() << " \n";
//                                     std::cout << "- int_exponend_bias + ac_error : "<< - int_exponend_bias + ac_error << " \n";

// ans = test1.data_ac_int() + test2.data_ac_int() - int_exponend_bias + ac_error;

//  std::cout << "The ans binary is : \n";
// for (int i = W-1; i>=0; i--){

//     std::cout<<ans[i];

// }
// std::cout<<"\n";



//  std::cout << "The ac_error binary is : \n";
// for (int i = W-1; i>=0; i--){

//     std::cout<<ac_error[i];

// }
// std::cout<<"\n";
// test_fin.set_data(ans);

//  std::cout << "test_fin is : "<< test_fin <<"\n";







// float test_number_a[N];
// float test_number_b[N];
// float test_ans = 0;


//  fast_log<W, E> array_a[N];
//  fast_log<W, E> array_b[N];
//  fast_log<W, E> answer = fast_log<W, E>(0);

// for (int i = 0; i < N; i++ ){
//     test_number_a[i] = generateRandomFloat();
//     std::cout << "The  test_number_a is :" <<  test_number_a[i] << "\n";
//     test_number_b[i] = generateRandomFloat();
//     std::cout << "The  test_number_b is :" <<  test_number_b[i] << "\n";
//     test_ans = test_ans + test_number_a[i] * test_number_b[i];

//      array_a[i] = test_number_a[i];
//      array_b[i] = test_number_b[i];

// }

// answer.dotProd<N>(array_a,array_b);

//     std::cout << "The result is :" << answer.num << "\n";

//     std::cout << "The correct result is :" << test_ans << "\n";



    return(0);
}