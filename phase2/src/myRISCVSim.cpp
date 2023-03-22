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
#include"self_defined_funcs.hpp"
#include "myRISCVSim.hpp"
#ifndef MYCLASSES
#define MYCLASSES
#include "registerfile.hpp"
#include "control_unit.hpp"
#endif
#include "global_variables.hpp"
using namespace std;

void fetch();
void decode();
void mA();
void write_back();
void display();
void positive_edge_trigger();
void run_riscvsim() {
    EXIT=false;
    int i;
    string run_mode="STEP";
    string input;
    cout<<"type STEP or RUN"<<endl;
    cin>>input;
    if(input=="RUN"){
        run_mode="RUN";
    }
    while(1){
        fetch();
        decode();
        if(EXIT){
            EXIT=false;
            display();
            return;
        }
        execute();
        mA();
        write_back();
        positive_edge_trigger();
        if(run_mode=="STEP"){
            display();
            cout<<"type STEP or RUN"<<endl;
            cin>>input;
            if(input=="RUN"){
                run_mode="RUN";
            }
        }   
    }
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc()
{
    // set PC to zero
    PC = 0;
    nextPC = 0;
    branchPC = 0;
    EXIT = false;
    mem.clear();
    for (int i = 0; i < 32; i++)
    {
        registerFile.set_register(i, 0);
        if (i == 2)
        {
            // x[i] = 0x7FFFFFF0; // sp
            registerFile.set_register(i, strtol("0x7FFFFFF0", NULL, 16)); // sp
        }
        else if (i == 3)
        {
            // x[i] = 0x10000000; // gp
            registerFile.set_register(i, strtol("0x10000000", NULL, 16)); // gp
        }
    }
    if_de_rest.instruction = "";

    de_ex_rest.A = 0;
    de_ex_rest.B = 0;
    de_ex_rest.branch_target = 0;
    de_ex_rest.op2 = 0;
    de_ex_rest.rd = 0;

    ex_ma_rest.alu_result = 0;
    ex_ma_rest.op2 = 0;
    ex_ma_rest.rd = 0;

    ma_wb_rest.alu_result = 0;
    ma_wb_rest.ld_result = 0;
    ma_wb_rest.rd = 0;
}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char *file_name) {
    FILE *fp;
  unsigned int address, instruction;
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    printf("Error opening input mem file\n");
    exit(1);
  }
  while(fscanf(fp, "%x %x", &address, &instruction) != EOF) {
    memory_write((unsigned int) address,(unsigned long long int)instruction,4);
    // printf("%x %u\n",address,mem[(unsigned int) address]);//  
  }
  fclose(fp);

}

// //reads from the instruction memory and updates the instruction register
void fetch()
{
    if (ex_ma_rest.control.isBranchTaken)
    {
        PC = branchPC;
    }
    else
    {
        PC = nextPC;
    }
    nextPC = PC + 4;
    cout<<"\nFETCH STAGE"<<endl;
    printf("Current PC=%x ",PC);
    unsigned int instruct_dec = (unsigned int)memory_read((unsigned int)PC, 4);
    string instruction = dec2bin(instruct_dec);
    temp_if_de_rest.instruction = instruction;
    temp_if_de_rest.PC=PC;
    cout<<"Instruction  "<<temp_if_de_rest.instruction<<endl;
}
// //reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode(){
    cout<<"\nDECODE STAGE"<<endl;
        //setting the controls
    if_de_rest.new_control.set_instruction(if_de_rest.instruction);
    if_de_rest.new_control.build_control();
    if(if_de_rest.new_control.isexit){
        EXIT=true;
        return;
    }

    // getting destination register
    string rds=if_de_rest.instruction.substr(20,5);
    int rd=(int)unsgn_binaryToDecimal(rds);
    // getting source register 1
    string rs1s=if_de_rest.instruction.substr(12,5);
    int rs1=unsgn_binaryToDecimal(rs1s);
    //getting source register 2
    string rs2s=if_de_rest.instruction.substr(7,5);
    int rs2=unsgn_binaryToDecimal(rs2s);
    int imm=immediate(if_de_rest.instruction);

    temp_de_ex_rest.rd=rd;
    temp_de_ex_rest.A=registerFile.get_register(rs1);
    temp_de_ex_rest.op2=registerFile.get_register(rs2);
    temp_de_ex_rest.branch_target=imm;
    temp_de_ex_rest.PC=if_de_rest.PC;
    temp_de_ex_rest.control=if_de_rest.new_control;
    if(if_de_rest.new_control.isImmediate){
        temp_de_ex_rest.B=imm;
    }
    else{
        temp_de_ex_rest.B=registerFile.get_register(rs2);
    }
    printf("branch target :%d\n",temp_de_ex_rest.branch_target);//
    printf("A :%d\n",temp_de_ex_rest.A);//
    printf("B :%d\n",temp_de_ex_rest.B);//
    printf("op2 :%d\n",temp_de_ex_rest.op2);//
    printf("rd :%d\n",temp_de_ex_rest.rd); //
}


// //executes the ALU operation based on ALUop
void execute(){
    cout<<"\nEXECUTE STAGE"<<endl;
    long long int alu_result;
    alu_result=alu_unit(de_ex_rest.control.aluSignal);
    // printf("%d alu_result\n",alu_result);//
    if(de_ex_rest.control.branchSelect==0){
        //not jalr type
        branchPC=de_ex_rest.branch_target+PC;
    }
    else if(de_ex_rest.control.branchSelect==1){
        //if jalr then pc
        branchPC=alu_result;
    }
    if(de_ex_rest.control.branchSignal=="nbr"){
        de_ex_rest.control.setIsBranchTaken(false);
    }
    else if(de_ex_rest.control.branchSignal=="ubr"){
        de_ex_rest.control.setIsBranchTaken(true);
    }
    else{
        if(de_ex_rest.control.branchSignal=="beq"){
            if(alu_result==0){
                de_ex_rest.control.setIsBranchTaken(true);
            }
            else{
                de_ex_rest.control.setIsBranchTaken(false); 
            }   
        }
        else if(de_ex_rest.control.branchSignal=="bne"){
            if(alu_result!=0){
                de_ex_rest.control.setIsBranchTaken(true);
            }
            else{
                de_ex_rest.control.setIsBranchTaken(false); 
            }   
        }
        else if(de_ex_rest.control.branchSignal=="blt"){
            if(alu_result<0){
                de_ex_rest.control.setIsBranchTaken(true);
            }
            else{
                de_ex_rest.control.setIsBranchTaken(false); 
            }   
        }
        else if(de_ex_rest.control.branchSignal=="bge"){
            printf("Bge ##");//
            if(alu_result>=0){
                de_ex_rest.control.setIsBranchTaken(true);
            }
            else{
                de_ex_rest.control.setIsBranchTaken(false); 
            }   
        }    
    }
    if(de_ex_rest.control.isauipc){
        temp_ex_ma_rest.alu_result=alu_result+PC;
    }
    else{
        temp_ex_ma_rest.alu_result=alu_result;
    }
    temp_ex_ma_rest.op2=(unsigned int) de_ex_rest.op2;
    temp_ex_ma_rest.rd=(unsigned int) de_ex_rest.rd;
    temp_ex_ma_rest.PC=de_ex_rest.PC;
    temp_ex_ma_rest.control=de_ex_rest.control;
    printf("alu result :%u \n",temp_ex_ma_rest.alu_result);//
    printf("op2 : %u\n",temp_ex_ma_rest.op2);//
    printf("rd :%u\n",temp_ex_ma_rest.rd);//
    printf("Branch PC(in hex) = %x\n",branchPC);
}

// //perform the memory operation
void mA() {
    cout<<"\nMEMORY ACCESS STAGE"<<endl;
    unsigned int ldResult=0;
    char my_char;
    short int my_short_int;
    int my_int;

    //load operation
    if(ex_ma_rest.control.isLd){
        if(ex_ma_rest.control.nBytes==1){
            my_char=(char)memory_read((unsigned int)ex_ma_rest.alu_result,1);
            my_int=(int)my_char;
            ldResult=(unsigned int)my_int;
        }
        else if(ex_ma_rest.control.nBytes==2){
            my_short_int=(short int)memory_read((unsigned int)ex_ma_rest.alu_result,2);
            my_int=(int)my_short_int;
            ldResult=(unsigned int)my_int;
        }
        else if(ex_ma_rest.control.nBytes==4){
            ldResult=(int)memory_read((unsigned int)ex_ma_rest.alu_result,4);
        }
        else{
            cout<<"nBytes is "<<ex_ma_rest.control.nBytes<<"not supported"<<endl;
        }
    }
    else{
        ldResult=0;
    }

    //store operation
    if(ex_ma_rest.control.isSt){
        if(ex_ma_rest.control.nBytes==1){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,1);
        }
        else if(ex_ma_rest.control.nBytes==2){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,2);
        }
        else if(ex_ma_rest.control.nBytes==4){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,4);
        }
        else{
            cout<<"nBytes is "<<ex_ma_rest.control.nBytes<<"not supported"<<endl;
        }   
    }
    temp_ma_wb_rest.alu_result=ex_ma_rest.alu_result;
    temp_ma_wb_rest.ld_result=ldResult;
    temp_ma_wb_rest.rd=ex_ma_rest.rd;
    temp_ma_wb_rest.PC=ex_ma_rest.PC;
    temp_ma_wb_rest.control=ex_ma_rest.control;
    cout<<"LdResult :"<<temp_ma_wb_rest.ld_result<<endl;
    cout<<"rd :"<<temp_ma_wb_rest.rd<<endl;
}

// //writes the results back to register file
void write_back()
{   
    cout<<"\nWRITE BACK STAGE"<<endl;
    if (ma_wb_rest.control.isWb)
    {
        unsigned int wb_result = 0;
        if (ma_wb_rest.control.wbSignal == "alu")
        {
            wb_result = ma_wb_rest.alu_result;
        }
        else if (ma_wb_rest.control.wbSignal == "ld")
        {
            wb_result = ma_wb_rest.ld_result;
        }
        else if (ma_wb_rest.control.wbSignal == "pc+4")
        {
            wb_result = PC + 4;
        }
        else
        {
            cout << "error :undefined wbSignal" << endl;
        }
        registerFile.set_register(ma_wb_rest.rd, wb_result);
        cout << "rd: " << ma_wb_rest.rd << "\nvalue: " << wb_result << endl;
    }
}

void display(){
    int ext=0,set_rst=1;
    while(!ext){
        printf("\n\n**** DISPLAY **** \n\n");
        while(set_rst!=0){
            registerFile.print_registers();
            cout<<"press '1' to set register\n"<< "press '0' to exit registerfile:";
            cin>>set_rst;
            if(set_rst==1){
                int rs,val;
                cout<<"Enter the register index in Range(0,31):";
                cin>>rs;
                cout<<"Current value of register : "<<registerFile.get_register(rs)<<endl;
                cout<<"Enter the value to insert :";
                cin>>val;
                registerFile.set_register(rs,val);
                cout<<"register file updated"<<endl;
            }
        }
        ext=0,set_rst=1;
        int mem_op=1;
        int op=0;
        cout<<"You are in memory section"<<endl;
        while(mem_op){
            cout<<"PRESS \n'0':exit\n'1':memory_lookup\n'2':memory update\nYour Choice :";
            cin>>op;
            if(op==0){
                break;
            }
            else if(op==1){
                int s_addr,e_addr;
                printf("Enter the range in hexa decimal format \nfrom start to end separated by space\nEg. 0x10002000 0x1000200c\nEnter :");
                scanf("%x %x",&s_addr,&e_addr);
                for(int i=0;i<=(e_addr-s_addr)/4;i++){
                    printf("%X %d\n",s_addr+(i*4),(unsigned int)memory_read(s_addr+(i*4),4));
                }
            }
            else if(op==2){
                unsigned addr;
                int val;
                printf("Enter addr in hexa decimal format Eg. 0x10002000\nEnter :");
                scanf("%x",&addr);
                printf("Current value of memory\n%X : %lld\n",addr,memory_read(addr,4));
                cout<<"Enter the new value of memory in decimal:";
                scanf("%d",&val);
                memory_write(addr,val,4);
            }
            else{
                cout<<"make a valid choice"<<endl;
            }   
        }
        printf("\n\n**** Enter 0 to exit display ****:");
        int dis_cod;
        cin>>dis_cod;
        if(dis_cod==0){
            return;
        }
    }
    
}

void positive_edge_trigger(){
    if_de_rest=temp_if_de_rest;
    de_ex_rest=temp_de_ex_rest;
    ex_ma_rest=temp_ex_ma_rest;
    ma_wb_rest=temp_ma_wb_rest;
}