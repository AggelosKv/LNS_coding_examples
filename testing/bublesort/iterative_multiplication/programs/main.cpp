#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_int.h"
#include "float_and_log_op.h"

#include <random>

#include "mc_scverify.h"

#define Width 64 // Bit width for ac_int

typedef iter_mul_num<Width,true> T;


#pragma hls_design
template<int N>
void CCS_BLOCK(bubbleSort)(T inA[N]) {
    
  for (int i=0; i<N-1; i++){
    for (int j=0; j<N-i-1; j++) {
      if (inA[j] > inA[j+1]) {
        T temp = inA[j];
        inA[j] = inA[j+1];
        inA[j+1] = temp;
      } 
    }
  }
};

float random_float() {
  float HI = rand();
  float LO = -rand();
  return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
};

CCS_MAIN(int, char**) {
  
  const int NSIZE = 10;
  T inA[NSIZE];

  srand(time(NULL));

  for (int i=0; i<NSIZE; i++) {
    inA[i] =  random_float();
    std::cout << inA[i].to_float() << std::endl;
  }
  std::cout << std::endl;
  
  bubbleSort<NSIZE>(inA);

  for (int i=0; i<NSIZE; i++) {
    std::cout << inA[i].to_float() << std::endl;
  }

  CCS_RETURN(0);
}