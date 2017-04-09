# Project Proposal: Parallel Debugging Tool for OpenMP and CUDA
## Brenda Thayillam & Emily Tsui
---


### Summary
We will be creating a parallel debugging tool for C++ programs that use OpenMP or CUDA programs on GPUs, specifially analyzing arithmetic intensity, percentage peak performance, memory access patterns, and potential speedup. We will be using this on the GHC machines, and the user will be able to provide specifications for the machines to simulate it on.

### Background
Arithmetic intensity is a measure of floating-point operations performed in a given code relative to the amount of memory accesses that are required to perform these operations. It is also a function of the percentage of peak performance that the code achieves on the machine after taking into account the specifications of the machine. False sharing occurs when multiple threads write to different addresses, but the addresses happen to fall on the same cache line. This causes the cache line to bounce between processors because they are each seeking exclusive access to the cache line on every write. Potential speedup involves looking at the code and how it would scale in relation to the number of processors it is run on. Amdahl's Law gives a theoretical speedup of the execution of a task given the proportion of sequential to potentially parallel code and how it scales with number of processors. All of these concepts can be applied to determine performance problems that could be causing issues in parallel code.

### Challenge
In order to make the parallel debugging tool as useful as possible, we need to have a deep understanding of the concepts mentioned above and be able to analyze how different lines of code could affect performance. The memory access pattern will be especially hard to analyze, as we will need a thorough understanding in order to come up with a good method of analysis for it.

### Resources
- We will refer to 15-418 lecture slides and other relevant papers to more thoroughly understand parallel performance metrics
- We will be coding from scratch
- The project will be developed on GHC machines and locally

### Goals and Deliverables
#### Plan to achieve
- Good performance of the debugging tool
- Allows the user to provide specifications for the machine they would like to simulate on
- Analyzes arithmetic intensity of OpenMP and CUDA code
- Analyzes percentage peak performance of OpenMP and CUDA code
- Analyzes problem-constrained scaling of OpenMP and CUDA code
- Analyzes false sharing of OpenMP and CUDA code

#### Hope to achieve
- Analyzes other memory access patterns of OpenMP and CUDA code
- Analyzes workload balance of OpenMP and CUDA code

#### Deliverables
- A C++ program which can analyze parallel performance problem in OpenMP and CUDA code

### Platform Choice
The code will be written in C++ because we plan to use Open MP to gain good performance in our code. We will be developing on the GHC machines because our code should not be too compute intensive.

### Schedule
- April 10 – 16: Read up on performance measurement techniques & create test harness with 10 programs with known performance metrics (OpenMP & CUDA)
- April 17 – 23: Complete sequential analyzer with dependency DAG generator
- April 24 – 30: Parallelize analysis tool for better performance
- May 1 – 7: Complete parallel implementation. Analyze performance relative to sequential. Attempt to create load analyzer (if time)
- May 8 – 12: Finish up and finalize project
