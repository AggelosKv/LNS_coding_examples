#include <iostream>
#include <cstdlib>
#include <cmath>
#include "ac_fixed.h"
#include "piecewise_approx_ax_plus_b.h"
#include <random>

#include "mc_scverify.h"

#define Width 64 // Bit width for ac_fixed
#define E 32  // Integer bits for ac_fixed


typedef fast_log<Width,E,true> T;


template<int N, int M>
void CCS_BLOCK(matXvec)(T A[N][M], T V[M], T O[N]) {
  
  #pragma hls_pipeline_init_interval 1
  for (int i=0; i<N; i++) {
    T sum = 0.0;

    sum.dotProd<M>(A[i],V);

    O[i] = sum;
  }
};

#pragma hls_design
template<int N, int M, int K>
void CCS_BLOCK(matXmat) (T A[N][M], T B[M][K], T O[N][K]) {
  for (int i=0; i<K; i++) {
    T V[M], OV[N];
    
    #pragma hls_unroll
    for (int j=0; j<M; j++)
      V[j] = B[j][i];

    matXvec<N,M>(A,V,OV);
    
    #pragma hls_unroll
    for (int j=0; j<N; j++)
      O[j][i] = OV[j];
  }
};

float random_float() {
  float HI = (rand())%100000;
  float LO = -(rand())%100000;
  return (LO + HI)/2;
};

int main(int argc, char** argv) {
  
  const int N=4, M=4, K=2;

  T A[N][M], B[M][K], OM[N][K];

  srand(time(NULL));
 
  for (int j=0; j<M; j++) {
    for (int i=0; i<N; i++)
      A[i][j] = random_float();
   
    for (int i=0; i<K; i++) 
      B[j][i] = random_float();
  }

  matXmat<N,M,K>(A,B,OM);
  
  std::cout << "\nMATRIX x MATRIX\n" << std::endl;
  for (int i=0; i<N; i++){
    for (int j=0; j<K; j++) {
      if (OM[i][j].sign)
        std::cout << OM[i][j].to_float() << "   ";
      else
        std::cout << OM[i][j].to_float() << "    ";
    }
    std::cout << std::endl;
  }

  return(0);
}