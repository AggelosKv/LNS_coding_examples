# Batch Normalization implementation

This is an example implementation of a Batch Normalization using the float_arithmetics library. The implementation can be found in the batch_normalization.cpp file together with the testbench main function. The batch normalization function is a templatized C++ function implemented for HLS using the Catapult HLS tool. The size of the input array to be sorted can be selected through the template parameter ''SIZE''. 

To synthesize the design on Catapult HLS use the *go_hls.tcl* script. The given example synthesizes a batch normalization algorithm of an array of 10 elements. 

```c++
const int SIZE = 10;
batch_norm<SIZE>(inA);
```

you should also change the value of the variable ''ARRAY_SIZE'' at the top of the *go_hls.tcl* TCL script.

```tcl
set ARRAY_SIZE 10
```

To synthesize the design launch catapult in the directory of the example.

```bash
catapult -f go_hls.tcl
```

