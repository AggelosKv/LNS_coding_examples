#include <iostream>
#include <cstdlib>
#include <cmath>
#include "piecewice_approx.h"
#include <random>

#include "mc_scverify.h"

#define W 64 // complete width of ac_std_float
#define E 32  // Exponent width of ac_std_float

typedef fast_log<W,E> T;


#pragma hls_design
template<int N>
void CCS_BLOCK(batch_norm)(T epsilon,T x[N], T out[N]) {
    
    static T mean,variance,temp,divider;

    for (int i=0; i<N; i++) {
      mean += x[i];  
    }
    mean = mean/N;

    for (int i=0; i<N; i++) {
      temp = (x[i] - mean);  
      
      temp.sq();
      variance +=temp;

    }
    variance = variance/N;
    divider = (variance + epsilon);
    divider.sqrt();


    for (int i=0; i<N; i++) {
      temp = x[i] - mean;
      out[i] = (temp)/(divider); 
    }  

};


template<int N>
void refbatch_norm(float epsilon,float x[N], float out[N]) {
    
    static float mean,variance,divider;

    for (int i=0; i<N; i++) {
      mean += x[i];  
    }
    mean = mean/N;

    for (int i=0; i<N; i++) {
      variance += (x[i] - mean)*(x[i] - mean);  
    }
    variance = variance/N;

    divider = sqrt(variance + epsilon);
    for (int i=0; i<N; i++) {
      out[i] = (x[i] - mean)/(divider); 
    }  



};

float random_float(float maxValue = 2000.0) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Define the range for floating-point numbers
    std::uniform_real_distribution<float> dist(-maxValue, maxValue);
    
    return dist(gen);
}

CCS_MAIN(int argc, char * argv[]) {
        
    const int SIZE = 10;

    float refarray_in[SIZE],refarray_out[SIZE], refepsilon;
    T array_in[SIZE],array_out[SIZE],epsilon;
    
    srand(time(NULL));

    refepsilon = 1;
    epsilon = refepsilon;

    for (int i=0; i<SIZE; i++) {
      refarray_in[i] = random_float(); 
      array_in[i] = refarray_in[i];

        std::cout << "In: " << array_in[i].to_float() << std::endl;
        std::cout << "REF_in: " << refarray_in[i] << "\n" << std::endl;

    }

      batch_norm<SIZE>(epsilon,array_in,array_out);
        std::cout << "\n" << std::endl;
      refbatch_norm<SIZE>(refepsilon,refarray_in,refarray_out);
    
    for (int i=0; i<SIZE; i++){
        std::cout << "OUT: " << array_out[i].to_float() << std::endl;
        std::cout << "REF_OUT: " << refarray_out[i] << "\n" << std::endl;
    }

    CCS_RETURN(0);
}