# RISCV-simulator
 These project is divided in three phases.
 Phase-I   Single Cycle Execution.

 How to execute:

 Description:
 The project's sorce code contains a number of c++ and header files. Each file having it's different function to do.In phase 1 the RISCV simulator will take one instruction at a time as it contains no pipelining.
 The RISCV simulator have 5 main stages:
 1. Fetch: It's main function is to take next instruction and update PC.
 2. Decode: It will decode the fetched instruction and assign different values to source register, destination registor , func3 , func7, immediate,etc.
 3. Execute: It will do the main work of comparing and calculations as it contain the arithematic and logical unit and passes the information to memory.
 4. Memory: It function is to read and write from memory.
 5. Write back: It will make the necessary changes in the register file, PC and call for next instruction as PC will be updated after this step.

 Team members:
 Chavda jayrajsinh : 2021CSB1078
 Khushboo gupta : 2021CSB1105
 Nikhil garg : 2021CSB1114
 Priyanshu kumar : 2021CSB1125

 Phase-II  Pipelined Execution.

 Yet to bo done 

 Phase-III Memory Hierarchy

 Yet to be done