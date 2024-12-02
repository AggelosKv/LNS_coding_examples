#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_int.h"
#include "float_and_log_op.h"

#include <random>

#include "mc_scverify.h"

#define W 64 // Bit width for ac_int
#define N 20  // Integer bits for ac_int


int generateRandomInt(int maxValue = 20000.0) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for int numbers
    std::uniform_real_distribution<float> dist(-maxValue, maxValue);
    
    return int(dist(gen));
}


int main(int argc, char* argv[]) {

    // Create iter_mul_num instances
    iter_mul_num<W,true> iter_mul_num_num1(0);
    iter_mul_num<W,true> iter_mul_num_num2(0);
    iter_mul_num<W,true> iter_mul_num_ans(0);
    int integer_num;

    int max_iterations = 1;

    int num1;
    int num2;
    int correct;
    int error;

    bool error_flag = false;



    for (int k = 0; k < max_iterations; k++){
        // Generate and print a random number below 20000
    num1 = generateRandomInt();
    num2 = generateRandomInt();

     
    correct = num1 * num2;
    error = 0.1*correct;

    std::cout << "Random number num1: " << num1 << std::endl;
    std::cout << "Random number num2: " << num2 << std::endl;

    iter_mul_num_num1 = num1;
    iter_mul_num_num2 = num2;

    iter_mul_num_ans = iter_mul_num_num1 * iter_mul_num_num2;


    std::cout << "The correct result is :" << correct << "\n";
    std::cout << "The given correct result is :" << iter_mul_num_ans.to_float() << "\n";
    correct = correct - iter_mul_num_ans.to_float();    // calculate the error (we could use double for better accuracy)
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




// int test_number_a[N];
// int test_number_b[N];
// int test_ans = 0;


//  iter_mul_num<W,true> array_a[N];
//  iter_mul_num<W,true> array_b[N];
//  iter_mul_num<W,true> answer = iter_mul_num<W,true>(0);

// for (int i = 0; i < N; i++ ){
//     test_number_a[i] = generateRandomInt();
//     std::cout << "The  test_number_a is :" <<  test_number_a[i] << "\n";
//     test_number_b[i] = generateRandomInt();
//     std::cout << "The  test_number_b is :" <<  test_number_b[i] << "\n";
//     test_ans = test_ans + test_number_a[i] * test_number_b[i];

//     array_a[i] =  iter_mul_num<W,true>(test_number_a[i]);
//     array_b[i] =  iter_mul_num<W,true>(test_number_b[i]);

// }



// answer.dotProd<N>(array_a,array_b);

// //answer = array_a[0] + array_b[0];
//     std::cout << "The result is :" << answer.to_float() << "\n";

//     std::cout << "The given correct result is :" << test_ans << "\n";



    return(0);
}