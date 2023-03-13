/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developer's Name:
Developer's Email id:
Date: 

*/


/* myRISCVSim.cpp
   Purpose of this file: implementation file for myRISCVSim
*/
#include "builtin_funcs.hpp"
#include "self_defined_funcs.hpp"
#include "myRISCVSim.hpp"

void fetch();
void decode();
void mA();
void write_back();

void run_riscvsim() {
    fetch();
    decode();
    mA();
    write_back();
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc() {

}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char *file_name) {

}

// //reads from the instruction memory and updates the instruction register
void fetch()
{
    if (mycontrol_unit.isBranchTaken)
    {
        PC = branchPC;
    }
    else
    {
        PC = nextPC;
    }
    nextPC = PC + 4;
    // printf("PC=%x\n",PC);
    unsigned int instruct_dec = (unsigned int)memory_read((unsigned int)PC, 4);
    // printf("%x ##\n",instruct_dec);//
    string instruction = dec2bin(instruct_dec);
    if_de_rest.instruction = instruction;
    // cout<<if_de_rest.instruction<<" ##"<<endl;////
}
// //reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode(){

}


// //executes the ALU operation based on ALUop
void execute(){

}

// //perform the memory operation
void mA() {

}

// //writes the results back to register file
void write_back() {

}