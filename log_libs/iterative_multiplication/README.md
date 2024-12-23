# ITERATIVE_MULTIPLICATION
Iterative_multiplication is a C++ header only library for integer arithmetic multiplication developed to be used for High Level Synthesis implemetations. It is based on "An iterative logarithmic multiplier" paper of Z. Babic, A. Avramovic, P. Bulic available in (http://dx.doi.org/10.1016/j.micpro.2010.07.001). The intention of that paper to make the multiplication operation faster using a convertion to the logarithmic domain. The target is to make multiplications without the need of the multiplication unit with the disadvantage of a possible small error or not. Τhe integer representation is used.


# Explaination
Inside the header file <iterative_multiplication.h> is described a templated datatype 'iter_mul_num<W,E>' together with a set of operations. The datatype supports different integer widths and signed or unsinged representation depending on the definitions of the widht W,sign T and I the number of available iterations of the multiplication operation through the template.

*NOTE: The more the available iterations of the multiplication operations help the algorithm to find the correct answer (error <= 1 (one integer)) even in the most difficult multiplications (Huge numbers need possibly more iterations).


Iterative_multiplication depends on ac_int that is available in [HLSLibs](https://github.com/hlslibs/ac_types).

Also the post-synthesis RTL co-simultion of the given examples require the sc_verify flow of Catapult HLS. The necessary header 
(mc_scverify) is publicly available in [ac_simutils](https://github.com/hlslibs/ac_simutils/tree/master/include).

# Supported Operators

* Addition (A+B)
* Substraction (A-B)
* Multiplication (A*B)
* Multiply-Add (A*B + C)
* Vector Dot Product (A[0]*B[0]+A[1]*B[1]+...+A[N-1]*B[N-1])

The above operations are overloaded to +,- and * operators. The same holds for comparisons and assignment operators

It should be noted that all the above operations are implementations proposed by the paper mention above except addition (+) and substraction (-). Those where added for completment.
