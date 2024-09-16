# C SIMD Utils

Implementation of simple utilities like searching in array using simd

## Required instruction set:
* x86 with SSE 4.2 for every _simd function
* x86 with AVX for every _avx function

## Valid usecases:
* searching ascii/ UTF-8 strings with length greater than 16 chars 
* array of bytes with length greater than 16 and known size

## Compile:

```
    gcc test.c -mavx -march=haswell -msse4.2 -Wall -pedantic -Wextra 
```