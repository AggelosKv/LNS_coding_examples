#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_fixed.h"
#include "piecewice_approx.h"
#include "ac_channel.h"
#include <random>

#include "mc_scverify.h"

#define Width 64 // Bit width for ac_fixed
#define I 32  // Integer bits for ac_fixed

typedef fast_log<Width,I,true> T;

// Implementation of an FIR filter using the fast-float library
#pragma hls_design
#pragma hls_pipeline_init_interval 1
template<int N>
void fir(T inp, T coeff[N], T &out) {
    
    static T x[N], sum;

    #pragma hls_unroll
    for (int i=N-1; i>0; i--) {
      x[i] = x[i-1];  
    }
    x[0] = inp;

    sum = 0.0;
    #pragma hls_unroll
    for (int i=0; i<N; i++) {
        sum +=  x[i]*coeff[i];
    }
    out = sum;
};

// Reference FIR filter using C++ float datatype
template<int N>
void refFir(float inp, float coeff[N], float &out) {
    
    static float x[N];
    float sum;

    for (int i=N-1; i>0; i--) {
      x[i] = x[i-1];  
    }
    x[0] = inp;

    sum = 0;
    for (int i=0; i<N; i++) {
        sum += x[i] * coeff[i];
    }
    out = sum;
};

float random_float() {
  float HI = (rand())%100000;
  float LO = -(rand())%100000;
  return (LO + HI)/2;
};


int main(int argc, char * argv[]) {
        
    const int TAPS = 5;
    const int ITER = 10;

    float refIn, refCoeff[TAPS], refOut;
    T In, Coeff[TAPS], Out;
    
    srand(time(NULL));

    for (int i=0; i<TAPS; i++) {
      refCoeff[i] = random_float(); 
      Coeff[i] = refCoeff[i];
    }

    for (int i=0; i<ITER; i++) {
      refIn =  random_float();
      In = refIn;

      fir<TAPS>(In,Coeff,Out);
      refFir<TAPS>(refIn,refCoeff,refOut);

      std::cout << "OUT: " << Out.to_float() << std::endl;
      std::cout << "REF: " << refOut << "\n" << std::endl;
    }
        
    return(0);
}