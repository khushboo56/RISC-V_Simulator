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
#ifndef MYCLASSES
#define MYCLASSES
#include "registerfile.hpp"
#include "control_unit.hpp"
#endif
#include "global_variables.hpp"


void fetch();
void decode();
void mA();
void write_back();

void run_riscvsim() {
    EXIT=false;
    while(1) {
        fetch();
        decode();
        if(EXIT){
            EXIT=false;
            printf("ORIGINAL\n");
            unsigned int addr=0x10001000;
            for(int i=0;i<10;i++){
                printf("%d. %d\n",i+1,(unsigned int)memory_read(addr,4));
                addr+=4;
            }
            printf("SORTED\n");
            addr=0x10002000;
            for(int i=0;i<10;i++){
                printf("%d. %d\n",i+1,(unsigned int)memory_read(addr,4));
                addr+=4;
            }
            return;
        }
        execute();
        mA();
        write_back();
    }
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
        //setting the controls
    mycontrol_unit.set_instruction(if_de_rest.instruction);
    mycontrol_unit.build_control();
    if(mycontrol_unit.isexit){
        EXIT=true;
        return;
    }

    // getting destination register
    string rds=if_de_rest.instruction.substr(20,5);
    cout<<if_de_rest.instruction<<" ##"<<endl;//
    cout<<rds<<"**$**"<<endl;//
    int rd=(int)unsgn_binaryToDecimal(rds);
    // getting source register 1
    string rs1s=if_de_rest.instruction.substr(12,5);
    int rs1=unsgn_binaryToDecimal(rs1s);
    //getting source register 2
    string rs2s=if_de_rest.instruction.substr(7,5);
    int rs2=unsgn_binaryToDecimal(rs2s);
    int imm=immediate(if_de_rest.instruction);

    de_ex_rest.rd=rd;
    de_ex_rest.A=registerFile.get_register(rs1);
    de_ex_rest.op2=registerFile.get_register(rs2);
    de_ex_rest.branch_target=imm;

    if(mycontrol_unit.isImmediate){
        de_ex_rest.B=imm;
    }
    else{
        de_ex_rest.B=registerFile.get_register(rs2);
    }
    // printf("branch target :%d\n",de_ex_rest.branch_target);//
    // printf("A :%d\n",de_ex_rest.A);//
    // printf("B :%d\n",de_ex_rest.B);//
    // printf("op2 :%d\n",de_ex_rest.op2);//
    // printf("rd :%d\n",de_ex_rest.rd); //

}


// //executes the ALU operation based on ALUop
void execute(){

}

// //perform the memory operation
void mA() {
    unsigned int ldResult=0;
    char my_char;
    short int my_short_int;
    int my_int;

    //load operation
    if(mycontrol_unit.isLd){
        if(mycontrol_unit.nBytes==1){
            my_char=(char)memory_read((unsigned int)ex_ma_rest.alu_result,1);
            my_int=(int)my_char;
            ldResult=(unsigned int)my_int;
        }
        else if(mycontrol_unit.nBytes==2){
            my_short_int=(short int)memory_read((unsigned int)ex_ma_rest.alu_result,2);
            my_int=(int)my_short_int;
            ldResult=(unsigned int)my_int;
        }
        else if(mycontrol_unit.nBytes==4){
            ldResult=(int)memory_read((unsigned int)ex_ma_rest.alu_result,4);
        }
        else{
            cout<<"nBytes is "<<mycontrol_unit.nBytes<<"not supported"<<endl;
        }
    }
    else{
        ldResult=0;
    }

    //store operation
    if(mycontrol_unit.isSt){
        if(mycontrol_unit.nBytes==1){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,1);
        }
        else if(mycontrol_unit.nBytes==2){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,2);
        }
        else if(mycontrol_unit.nBytes==4){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,4);
        }
        else{
            cout<<"nBytes is "<<mycontrol_unit.nBytes<<"not supported"<<endl;
        }   
    }
    ma_wb_rest.alu_result=ex_ma_rest.alu_result;
    ma_wb_rest.ld_result=ldResult;
    ma_wb_rest.rd=ex_ma_rest.rd;
}

// //writes the results back to register file
void write_back() {

}