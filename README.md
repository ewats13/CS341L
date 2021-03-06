# CACHE LAB

This assignment required us to write a Cache Simulator from scratch (csim.c) along with a matrix transpose function (trans.c) that is optimized for cache performance.
The overview and requirements of this assignment are specified in the PDF "CacheLab_INFO"


######Running the autograders:


Before running the autograders, compile your code:
```
linux> make
```
Check the correctness of your simulator:
```
linux> ./test-csim
```
Check the correctness and performance of your transpose functions:
```
linux> ./test-trans -M 32 -N 32
linux> ./test-trans -M 64 -N 64
linux> ./test-trans -M 61 -N 67
```
Check everything at once (this is the program that the instructor ran while grading):
```
linux> ./driver.py    
```

######Files:

These files were written by me, and handed in for the assignment <br />
- csim.c       - My cache simulator 
- trans.c      - My transpose function

Tools for evaluating the simulator and transpose function
- Makefile     - Builds the simulator and tools
- README       - This file
- driver.py*   - The driver program, runs test-csim and test-trans
- cachelab.c   - Required helper functions
- cachelab.h   - Required header file
- csim-ref*    - The executable reference cache simulator
- test-csim*   - Tests the cache simulator
- test-trans.c - Tests the transpose function
- tracegen.c   - Helper program used by test-trans
- traces/      - Trace files used by test-csim.c
