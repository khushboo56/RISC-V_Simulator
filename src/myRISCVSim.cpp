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
void fetch() {

}

// //reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode(){

}


// //executes the ALU operation based on ALUop
void execute(){
    long long int alu_result;
    alu_result=alu_unit(mycontrol_unit.aluSignal);
    // printf("%d alu_result\n",alu_result);//
    if(mycontrol_unit.branchSelect==0){
        //not jalr type
        branchPC=de_ex_rest.branch_target+PC;
    }
    else if(mycontrol_unit.branchSelect==1){
        //if jalr then pc
        branchPC=alu_result;
    }
    if(mycontrol_unit.branchSignal=="nbr"){
        mycontrol_unit.setIsBranchTaken(false);
    }
    else if(mycontrol_unit.branchSignal=="ubr"){
        mycontrol_unit.setIsBranchTaken(true);
    }
    else{
        if(mycontrol_unit.branchSignal=="beq"){
            if(alu_result==0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }
        else if(mycontrol_unit.branchSignal=="bne"){
            if(alu_result!=0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }
        else if(mycontrol_unit.branchSignal=="blt"){
            if(alu_result<0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }
        else if(mycontrol_unit.branchSignal=="bge"){
            printf("Bge ##");//
            if(alu_result>=0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }    
    }
    ex_ma_rest.alu_result=alu_result;
    ex_ma_rest.op2=(unsigned int) de_ex_rest.op2;
    ex_ma_rest.rd=(unsigned int) de_ex_rest.rd;
     printf("alu result :%u \n",ex_ma_rest.alu_result);//
     printf("op2 : %u\n",ex_ma_rest.op2);//
     printf("rd :%u\n",ex_ma_rest.rd);//
}

// //perform the memory operation
void mA() {

}

// //writes the results back to register file
void write_back() {

}