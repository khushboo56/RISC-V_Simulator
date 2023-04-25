# RISCV-simulator
 These project is divided in three phases.
 Phase-I   Single Cycle Execution.


 How to execute:


 We have to give the following command in terminal to run our code: 
 g++ main.cpp alu_unit.cpp control_unit.cpp dec2bin.cpp global_variables.cpp immediate.cpp memory_read_write_funcs.cpp myRISCVSim.cpp registerfile.cpp -o output.out


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




 We have to give the following command in terminal to run our code: 
 g++ main.cpp alu_unit.cpp control_unit.cpp dec2bin.cpp global_variables.cpp immediate.cpp memory_read_write_funcs.cpp myRISCVSim.cpp registerfile.cpp -o output.out


 Then we should give the following command after the above command:
 ./output.out <filename.mc>
 where in brackets we have to write name of the file which contain our assembly code


 To exit the RISCV simulator we have used opcode="1111111"


 We have worked on our  Phase 1 code, and make it pipelined.
We have. Introduced pipeline registers/inter-stage buffers between each of the execution steps.
We have  changed the data path and control path to support pipelining.
 We have introduced logic of data forwarding(forwarding unit), stalling, flushing the pipeline 
For control hazards,we have  included one-bit branch prediction logic(Branch Predictor).


While running the file:
When you run the file,
1. First it will ask the user whether to use Pipelining or non pipelining (Knob1)
   1. Enter 0 for Non pipelining or 1 for Pipelining
2. Next , it will ask whether to use stalling in pipelining or not.(knob2)
   1. Enter 0 for if pipelining is expected to work with stalling. And 1 for Data forwarding..
3. Next it will ask whether to print values of the registers at the end of each cycle or not.(knob3)
   1. Enter 0 for not printing and 1 for printing values of registers at the end of each file.
4. Next it will ask whether to Enable/disable printing information in the pipeline registers at the end of each cycle(similar to tracing), along with cycle number or not(knob4)
   1. Enter 0 for not printing and 1 for printing.
5. Next Knob is to enable knob 4 for any specific instruction.With this feature we will be able to see the pipeline registers information for a particular instruction of our interest. Here, the instruction can be specified as number (example, if the instruction we are interestd in is the 10th instruction in the input program, 10 will be taken as input).
Then the code will run and give output accordingly.


Simulator Flow
There are two steps:
1. First memory is loaded with input memory file.
2. Simulator executes instruction one by one.
For the second step, there is infinite loop, which simulates all the instruction till the instruction sequence reads “0xFFFFFFFF”.






































 Phase-III Memory Hierarchy


 Yet to be done