# PIECEWISE APPROXIMATION
Pieciwise_aprox is a C++ header only library forfor floating point arithmetic operations developed to be used for High Level Synthesis implemetations. It is based on "A 231-MHz, 2.18-mW 32-bit Logarithmic Arithmetic Unit for Fixed-Point 3-D Graphics System" paper of Hyejung Kim, Byeong-Gyu Nam, Ju-Ho Sohn, Jeong-Ho Woo and Hoi-Jun Yoo available in (https://ieeexplore.ieee.org/document/4017592). The intention of that paper to make some of the arithmetic operations faster  using a convertion to the logarithmic domain. This is achived without the need of the multiplication unit with the disadvantage of a possible small error in the result due to the convertion. Τhe fixed point signed representation is used.


# Explaination
Inside the header file <piecewise_approx.h> is described a templated datatype 'fast_log<W,I>' together with a set of operations. The datatype supports different integer and fractional widths representation depending on the definitions of the widht W and integer width I through the template.


piecewise_approx depends on ac_fixed that is available in [HLSLibs](https://github.com/hlslibs/ac_types).

Also the post-synthesis RTL co-simultion of the given examples require the sc_verify flow of Catapult HLS. The necessary header 
(mc_scverify) is publicly available in [ac_simutils](https://github.com/hlslibs/ac_simutils/tree/master/include).

# Supported Operators

* Addition (A+B)
* Substraction (A-B)
* Multiplication (A*B)
* Division(A/B)
* Squere root (sqrt(A))
* Square (A^2)
* power of 2^n (A^(2^n)) 
* Multiply-Add (A*B + C)
* Vector Dot Product (A[0]*B[0]+A[1]*B[1]+...+A[N-1]*B[N-1])

The above operations are overloaded to +,-,/ and * operators. The same holds for comparisons and assignment operators

It should be noted that all the above operations are implementations proposed by the paper mention above except addition (+) and substraction (-). Those where added for completment.