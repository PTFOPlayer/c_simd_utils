# MEMCHR in C using SIMD

This is a simple implementation of memchr using SIMD extension.

## Required instruction set:
* x86 with SSE 4.2 

## Valid usecases:
* searching ascii/ UTF-8 strings with length greater than 16 chars 
* array of bytes with length greater than 16 and known size
