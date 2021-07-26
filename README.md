# BigIntCpp
Arbitrarily large precision BigInt Cpp Implementation.

## Description
BigIntCpp is a header only library containing signed (bigint.h) and unsigned (ubigint.h) big-integer implementations. It utilizes std::deque as the underlying storage mechanism.

This implementation is a starting point (not meant to be optimized) BigInt Cpp implementation with the purpose of demonstrating big-integer specific algorithms in modern cpp.

Possible future optimizations for BigIntCpp:

- FFT based multplication in certain cases
- Base power of 2 (currently using base-10)
- Optimizing implementation of underlying container (current std::deque for fast implementation)
  - Most notibly minimizing inserts/shifts


