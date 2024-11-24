#include <iostream>
#include <random>
#include "config.h"
#include "ac_fixed.h"
#include "float2log.h"
#include "log2float.h"
#include "util.h"

// Driver program to test the above function
int main(int argc, char* argv[]) {
    

    double random_num;
    double correct;
    double error;
    // 64 num of bits , 32 integer bits
    ac_fixed< W, I, true> num1;


    int fin = 1000;

    for (int i = 0; i<fin; i++){

    random_num = generateRandomDouble();
    
    num1 = ac_fixed< W, I, true>(random_num);


    std::cout << "This is the input " << num1<< "\n";

    // Convert the float to logarithm
    float2log(num1);

    std::cout << "This the output logarithm :" << num1<< "\n";

    // Convert the logarithm to  float
    log2float(num1);

    std::cout << "This the final output :" << num1<< "\n";
    correct = random_num;
    std::cout << "This the true output :" << correct<< "\n \n";

    error = (double)abs(0.246*correct);
    correct = correct - num1.to_double();

        if (abs(correct) > error){
            std::cout << "The error should be less than :"<< error <<" but it is : "<< abs(correct) << "\n";
            
            std::cout << "[ERROR]\n";
            break;
        }
    }
    return (0);
}