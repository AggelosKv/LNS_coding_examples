#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_fixed.h"
#include "float_and_log_op.h"

#include <random>

#include "mc_scverify.h"

#define W 64 // Bit width for ac_fixed
#define I 32  // Integer bits for ac_fixed
#define N 20  // Integer bits for ac_fixed


float generateRandomFloat(float maxValue = 200.0) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<float> dist(1, maxValue);
    
    return dist(gen);
}


int main(int argc, char* argv[]) {

    // //Create fast_log instances
    // fast_log<W, I,true> fast_log_num1(0);
    // fast_log<W, I,true> fast_log_num2(0);
    // fast_log<W, I,true> fast_log_ans(0);
    // float float_num;

    // int max_iterations = 10000;

    // float num1;
    // float num2;
    // float correct;
    // float error;

    // bool error_flag = false;



    // for (int k = 0; k < max_iterations; k++){
    //     // Generate and print a random floating-point number below 20000
    // num1 = generateRandomFloat();
    // num2 = generateRandomFloat();

     
    // correct = num1 * num2;
    // error = 0.246*correct;

    // std::cout << "Random floating-point number num1: " << num1 << std::endl;
    // std::cout << "Random floating-point number num2: " << num2<< std::endl;

    // fast_log_num1 = num1;
    // fast_log_num2 = num2;

    // fast_log_ans = fast_log_num1 * fast_log_num2;


    // std::cout << "The correct result is :" << correct << "\n";
    // std::cout << "The given correct result is :" << fast_log_ans.to_float() << "\n";
    // correct = correct - fast_log_ans.to_float();    // calculate the error (we could use double for better accuracy)
    // std::cout << "The error between them is :" << correct << "\n";
    // std::cout << std::endl;

    //     if (abs(correct) > abs(error)){
    //         std::cout << "The error should be less than :" << error <<" \n";
    //         std::cout << "[ERROR]\n";
    //         error_flag = true;
    //         break;
    //     }
    // }

    // if (!error_flag){
    //     std::cout << "No errorus results where detected! \n";
    // }



float test_number_a[N];
float test_number_b[N];
float test_ans = 0;


 fast_log<W, I,true> array_a[N];
 fast_log<W, I,true> array_b[N];
 fast_log<W, I,true> answer = fast_log<W, I,true>(0);

for (int i = 0; i < N; i++ ){
    test_number_a[i] = generateRandomFloat();
    std::cout << "The  test_number_a is :" <<  test_number_a[i] << "\n";
    test_number_b[i] = generateRandomFloat();
    std::cout << "The  test_number_b is :" <<  test_number_b[i] << "\n";
    test_ans = test_ans + test_number_a[i] * test_number_b[i];

    array_a[i] =  fast_log<W, I,true>(test_number_a[i]);
    array_b[i] =  fast_log<W, I,true>(test_number_b[i]);

}



answer.dotProd<N>(array_a,array_b);

//answer = array_a[0] + array_b[0];
    std::cout << "The  result is :" << answer.to_float() << "\n";

    std::cout << "The given correct result is :" << test_ans << "\n";

    return(0);
}