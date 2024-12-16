#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_fixed.h"
#include "piecewise_approx_ax_plus_b.h"

#include <random>

#include "mc_scverify.h"

#define W 64 // Bit width for ac_fixed
#define I 32  // Integer bits for ac_fixed

typedef fast_log<W,I,true> T;

// Implementation of an FIR filter using the fast-float library
#pragma hls_design
#pragma hls_pipeline_init_interval 1


#pragma hls_design
template<int N>
void bubbleSort(T inA[N]) {
    
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

int main(int, char**) {
  
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

  return(0);
}