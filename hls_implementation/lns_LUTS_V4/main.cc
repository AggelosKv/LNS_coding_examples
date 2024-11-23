#include <iostream>
#include "ac_fixed.h"
#include "top_module.h"

#include <random>

#include "mc_scverify.h"

// total number of bits
const int W = 64;

// Total number of bits for integer representation
const int I = 32;

// Function to generate a random floating-point number between 0 and 20000
double generateRandomDouble(double maxValue = 20000.0) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<double> dist(1, maxValue);
    
    return dist(gen);
}


// Driver program to test the above function
int main(int argc, char* argv[]) {
    

    double random_num1;
    double random_num2;
    double correct;
    double error;
    // 64 num of bits , 32 integer bits
    ac_fixed< W, I, true> num1;
	ac_fixed< W, I, true> num2;
	
	ac_fixed< W, I, true> ans;

    int fin = 10000;

    for (int i = 0; i<fin; i++){

		random_num1 = generateRandomDouble();
		
		random_num2 = generateRandomDouble();
		
		num1 = ac_fixed< W, I, true>(random_num1);
		
		num2 = ac_fixed< W, I, true>(random_num2);

		std::cout << "This is the input num1 : " << num1<< "\n";
		
		std::cout << "This is the input num2 : " << num1<< "\n";

		top_module<W,I>(num1,num2,ans);
		
		//the correct value is returned to num1

		std::cout << "This the final output " << ans << "\n";
		correct = random_num1 * random_num2;
		std::cout << "This the true output " << correct<< "\n \n";

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
