# RISCV-simulator
 These project is divided in three phases.
 Phase-I   Single Cycle Execution.

 How to execute:

 We have to give the following command in terminal to run our code: 
 g++ main.cpp alu_unit.cpp control_unit.cpp dec2bin.cpp global_variables.cpp immediate.cpp memory_read_write_funcs.cpp myRISCVSim.cpp registerfile.cpp -o output.out

 Then we should give the following command after the above command:
 ./output.out <filename.mc>
 where in brackets we have to write name of the file which contain our assembly code

 To exit the RISCV simulator we have used opcode="1111111"

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