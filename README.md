# Introduction
Implementation of SM4 and a variety of its optimized version.
The following optimization strategy is used:

- Loop unrolling
- Multithread
- Look up table
- SIMD(AVX ISA)


# Optimization strategy

## Algorithm Level
Implement T transform by Look up Table Algorithm.

## Thread Level
For a large number of encryption tasks, multi-threaded acceleration is used

## Compiler Level
- MSVC X64 Release
- Use the "gestalt" of loop unrolling to reduce instructions

## Data Level
SIMD(AVX) 


# Test
