# MITCHELL_APPROX
Mitchell_aprox is a C++ header only library for integer arithmetic multiplication developed to be used for High Level Synthesis implemetations. It is based on "Computer Multiplication and Division Using Binary Logarithms" paper of JOHN N. MITCHELL, JR.t ASSOCIATE, IRE available in (https://ieeexplore.ieee.org/document/5219391). The intention of that paper to make the multiplication operation faster using a convertion to the logarithmic domain.The target is to make multiplications without the need of the multiplication unit with the disadvantage of a possible small error in the result due to the convertion. Τhe fixed point signed representation is used.


# Explaination
Inside the header file <mitchell_aprox.h> is described a templated datatype 'fast_log<W,E>' together with a set of operations. The datatype supports different integer and fractional widths representation depending on the definitions of the widht W and integer width I through the template.


mitchell_aprox depends on ac_fixed that is available in [HLSLibs](https://github.com/hlslibs/ac_types).

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
