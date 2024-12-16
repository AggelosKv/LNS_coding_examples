# FLOAT_ARITHMETICS
Float_Arithmetics is a C++ header only library for floating point arithmetic operations developed to be used for High Level Synthesis implemetations. It is based on "Approximate Floaong-Point Operatons with Integer Units by Processing in the Logarithmic Domain" paper of Oscar Gustafsson and Noah Hellman available in (https://ieeexplore.ieee.org/document/9603384). The intention of that paper to make some of the arithmetic operations faster using a convertion to the logarithmic domain. This is achived without the need of the multiplication unit with the disadvantage of a small error in the result due to the convertion. Τhe floating representation (IEEE 754 binary format) is used.


# Explaination
Inside the header file <float_arithmetics.h> is described a templated datatype 'fast_log<W,E>' together with a set of operations. The datatype supports different floating point representations depending on the definitions of the widht W and exponent E widths through the template.


Float_Arithmetics depends on ac_std_float and ac_int libraries that are available in [HLSLibs](https://github.com/hlslibs/ac_types).

Also the post-synthesis RTL co-simultion of the given examples require the sc_verify flow of Catapult HLS. The necessary header 
(mc_scverify) is publicly available in [ac_simutils](https://github.com/hlslibs/ac_simutils/tree/master/include).

# Supported Operators

* Addition (A+B)
* Substraction (A-B)
* Multiplication (A*B)
* Division(A/B)
* Squere root (sqrt(A))
* Square (A^2)
* Reciprocal (1/A)
* Inverted sqrt (1/sqrt(A))
* Division sqrt (A/sqrt(B))
* Multiply-Add (A*B + C)
* Vector Dot Product (A[0]*B[0]+A[1]*B[1]+...+A[N-1]*B[N-1])

The above operations are overloaded to +,-,/ and * operators. The same holds for comparisons and assignment operators

It should be noted that all the above operations are implementations proposed by the paper mention above except addition (+) and substraction (-). Those where added for completment.



