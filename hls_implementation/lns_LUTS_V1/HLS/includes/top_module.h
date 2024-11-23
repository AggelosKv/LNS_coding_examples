#include "mc_scverify.h"

#pragma hls_design top
template <int W,int I>
void CCS_BLOCK(top_module)(ac_fixed< W, I, true> &float_num){

    // Convert the float to logarithm
    float2log<W,I>(num1);

    // Convert the logarithm to float
    log2float<W,I>(num1);
}