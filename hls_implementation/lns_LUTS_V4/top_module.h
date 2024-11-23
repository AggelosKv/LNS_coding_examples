#include "ac_fixed.h"
#include "log2float.h"
#include "float2log.h"
#include "mc_scverify.h"

#pragma hls_design top
template <int W,int I>
void CCS_BLOCK(top_module)(ac_fixed< W, I, true> &num1, ac_fixed< W, I, true> &num2 , ac_fixed< W, I, true> &out  ){
	
	ac_fixed< W, I, true> num1_out;
	ac_fixed< W, I, true> num2_out;
	ac_fixed< W, I, true> temp_out;
	
    // Convert the float to logarithm
    float2log<W,I>(num1,num1_out);
    
    // Convert the float to logarithm
    float2log<W,I>(num2,num2_out);
    
    temp_out = num1_out + num2_out; // num1 * num2

    // Convert the logarithm to float
    log2float<W,I>(temp_out,out);
}
