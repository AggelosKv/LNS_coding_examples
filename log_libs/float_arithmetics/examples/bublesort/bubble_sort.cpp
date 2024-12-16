#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_std_float.h"
#include "float_arithmetics.h"

#include <random>

#include "mc_scverify.h"

#define Width 32 // Bit width for ac_fixed
#define E 8  // Integer bits for ac_fixed
#define I W-E // 
typedef fast_log<Width,E> T;

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
    std::cout << inA[i].num << std::endl;
  }
  std::cout << std::endl;
  
  bubbleSort<NSIZE>(inA);

  for (int i=0; i<NSIZE; i++) {
    std::cout << inA[i].num << std::endl;
  }

  CCS_RETURN(0);
}