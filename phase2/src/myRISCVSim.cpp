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
#include "forwarding_unit.hpp"
#endif
#include "global_variables.hpp"
using namespace std;


void run_riscvsim(bool knob2) {
    display();//
    int cycle=1;//
    EXIT=false;
    int i;
    string run_mode="STEP";
    string input;
    cout<<"type STEP or RUN"<<endl;
    cin>>input;
    if(input=="RUN"){
        run_mode="RUN";
    }
    while(!EXIT){
        printf("*** CYCLE : %d  ***",cycle);//
        fetch();
        decode(knob2);
        // if(EXIT){
        //     EXIT=false;
        //     display();
        //     return;
        // }
        execute(knob2);
        mA(knob2);
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
        cycle++;//  
    }
    display();
    
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc()
{
    // set PC to zero
    PC = 0;
    PCWrite=true;
    nextPC = 4;
    branchPC = 0;
    EXIT = false;
    mem.clear();
    for(int i=0;i<BTB_SIZE;i++){
        BTB[i].address=-0xF;
    }
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
    Control_unit nop_control;
    nop_control.set_instruction("00000000000000000000000000000000");
    nop_control.build_control();

    if_de_rest.instruction = "00000000000000000000000000000000";
    if_de_rest.writemode=true;
    if_de_rest.PC=-1;
    temp_if_de_rest=if_de_rest;

    de_ex_rest.instruction=if_de_rest.instruction;
    de_ex_rest.branch_target = 0;
    de_ex_rest.A = 0;
    de_ex_rest.B = 0;
    de_ex_rest.op2 = 0;
    de_ex_rest.rd = 0;
    de_ex_rest.rs1=0;
    de_ex_rest.rs2=0;
    de_ex_rest.control=nop_control;
    de_ex_rest.PC=-1;
    de_ex_rest.writemode=true;
    temp_de_ex_rest=de_ex_rest;

    ex_ma_rest.instruction=if_de_rest.instruction;
    ex_ma_rest.alu_result=0;
    ex_ma_rest.op2=0;
    ex_ma_rest.rd=0;
    ex_ma_rest.rs1=0;
    ex_ma_rest.rs2=0;
    ex_ma_rest.control=nop_control;
    ex_ma_rest.PC=-1;
    ex_ma_rest.writemode=true;
    temp_ex_ma_rest=ex_ma_rest;

    ma_wb_rest.instruction=if_de_rest.instruction;
    ma_wb_rest.alu_result = 0;
    ma_wb_rest.ld_result = 0;
    ma_wb_rest.rd = 0;
    ma_wb_rest.rs1=0;
    ma_wb_rest.rs2=0;
    ma_wb_rest.control=nop_control;
    ma_wb_rest.PC=-1;
    ma_wb_rest.writemode=true;
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
    // nextPC = PC + 4;
    cout<<"\nFETCH STAGE"<<endl;
    printf("Current PC=%x ",PC);
    unsigned int instruct_dec = (unsigned int)memory_read((unsigned int)PC, 4);
    string instruction = dec2bin(instruct_dec);
    temp_if_de_rest.instruction = instruction;  //it is acting like a buffer register
    temp_if_de_rest.PC=PC;
    cout<<"Instruction  "<<temp_if_de_rest.instruction<<endl;
}
// //reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode(bool knob2){
    cout<<"\nDECODE STAGE"<<endl;
        //setting the controls
    if_de_rest.new_control.set_instruction(if_de_rest.instruction);
    if_de_rest.new_control.build_control();
    if(if_de_rest.new_control.isexit){
        PCWrite=false;
        if_de_rest.writemode=false;
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
    //setting the forwarding unit 
    forwarding_unit.de_inst.opcode=if_de_rest.new_control.inst_type;
    forwarding_unit.de_inst.rd=rd;
    forwarding_unit.de_inst.rs1=rs1;
    forwarding_unit.de_inst.rs2=rs2;

    forwarding_unit.ex_inst.opcode=de_ex_rest.control.inst_type;
    forwarding_unit.ex_inst.rd=de_ex_rest.rd;
    forwarding_unit.ex_inst.rs1=de_ex_rest.rs1;
    forwarding_unit.ex_inst.rs2=de_ex_rest.rs2; 

    forwarding_unit.ma_inst.opcode=ex_ma_rest.control.inst_type;
    forwarding_unit.ma_inst.rd=ex_ma_rest.rd;
    forwarding_unit.ma_inst.rs1=ex_ma_rest.rs1;
    forwarding_unit.ma_inst.rs2=ex_ma_rest.rs2; 

    forwarding_unit.wb_inst.opcode=ma_wb_rest.control.inst_type;
    forwarding_unit.wb_inst.rd=ma_wb_rest.rd;
    forwarding_unit.wb_inst.rs1=ma_wb_rest.rs1;
    forwarding_unit.wb_inst.rs2=ma_wb_rest.rs2;

    if(knob2){ 
        int op1=registerFile.get_register(rs1),op2=registerFile.get_register(rs2);
        forwarding_unit.build_mux_selectors();
        printf("select_de_op1: %d\n",forwarding_unit.select_de_op1);
        printf("select_de_op2: %d\n",forwarding_unit.select_de_op2);
        printf("select_ex_A: %d\n",forwarding_unit.select_ex_A);
        printf("select_ex_B: %d\n",forwarding_unit.select_ex_B);
        printf("select_ex_op2: %d\n",forwarding_unit.select_ex_op2);
        printf("select_ma_op2: %d\n",forwarding_unit.select_ma_op2);
        // printf("selector")
        temp_de_ex_rest.instruction=if_de_rest.instruction;
        temp_de_ex_rest.rd=rd;
        
        //forwarding
        unsigned int wb_result = 0;
        if (ma_wb_rest.control.isWb)
        {
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
                wb_result = ma_wb_rest.PC + 4;
            }
            else
            {
                cout << "error :undefined wbSignal" << endl;
            }
        }
        //mux1
        if(forwarding_unit.select_de_op1==0){
            temp_de_ex_rest.A=op1;
        }
        else if(forwarding_unit.select_de_op1==1){
            temp_de_ex_rest.A=wb_result;
        }
        else{
            cout<<"unidentified value of select_de_op1"<<endl;
        }

        //mux2
        if(forwarding_unit.select_de_op2==0){
            temp_de_ex_rest.op2=op2;
        }
        else if(forwarding_unit.select_de_op2==1){
            temp_de_ex_rest.op2=wb_result;
        }
        else{
            cout<<"unidentified value of select_de_op2"<<endl;
        }


        
        
        temp_de_ex_rest.branch_target=imm;
        temp_de_ex_rest.PC=if_de_rest.PC;
        temp_de_ex_rest.control=if_de_rest.new_control;
        if(if_de_rest.new_control.isImmediate){
            temp_de_ex_rest.B=imm;
        }
        else{
            temp_de_ex_rest.B=temp_de_ex_rest.op2;
        }
        temp_de_ex_rest.rs1=rs1;
        temp_de_ex_rest.rs2=rs2;    
    }
    else{
        if(forwarding_unit.ifDependencyrs1(forwarding_unit.de_inst,forwarding_unit.ex_inst)
        ||forwarding_unit.ifDependencyrs2(forwarding_unit.de_inst,forwarding_unit.ex_inst)
        ||forwarding_unit.ifDependencyrs1(forwarding_unit.de_inst,forwarding_unit.ma_inst)
        ||forwarding_unit.ifDependencyrs2(forwarding_unit.de_inst,forwarding_unit.ma_inst)
        ||forwarding_unit.ifDependencyrs1(forwarding_unit.de_inst,forwarding_unit.wb_inst)
        ||forwarding_unit.ifDependencyrs2(forwarding_unit.de_inst,forwarding_unit.wb_inst)
        ){
            PCWrite=false;
            if_de_rest.writemode=false;
            
            temp_de_ex_rest.instruction="00000000000000000000000000000000";
            temp_de_ex_rest.rd=0;
            temp_de_ex_rest.A=0;
            temp_de_ex_rest.B=0;
            temp_de_ex_rest.op2=0;
            temp_de_ex_rest.branch_target=0;
            temp_de_ex_rest.PC=0;
            temp_de_ex_rest.control.set_instruction(temp_de_ex_rest.instruction);
            temp_de_ex_rest.control.build_control();
            temp_de_ex_rest.rs1=0;
            temp_de_ex_rest.rs2=0;
            return;
            cout<<"nop :stall due to data dependency(no forwarding)";
        }
        else{
            temp_de_ex_rest.instruction=if_de_rest.instruction;
            temp_de_ex_rest.rd=rd;
            temp_de_ex_rest.A=registerFile.get_register(rs1);
            temp_de_ex_rest.op2=registerFile.get_register(rs2);
            temp_de_ex_rest.branch_target=imm;
            temp_de_ex_rest.PC=if_de_rest.PC;
            temp_de_ex_rest.control=if_de_rest.new_control;
            temp_de_ex_rest.rs1=rs1;
            temp_de_ex_rest.rs2=rs2;
            if(if_de_rest.new_control.isImmediate){
                temp_de_ex_rest.B=imm;
            }
            else{
                temp_de_ex_rest.B=registerFile.get_register(rs2);
            }
        }
    }
    
    printf("branch target :%d\n",temp_de_ex_rest.branch_target);//
    printf("A :%d\n",temp_de_ex_rest.A);//
    printf("B :%d\n",temp_de_ex_rest.B);//
    printf("op2 :%d\n",temp_de_ex_rest.op2);//
    printf("rd :%d\n",temp_de_ex_rest.rd); //
}


// //executes the ALU operation based on ALUop
void execute(bool knob2){
    //forwarding
    if(de_ex_rest.control.isexit){
        de_ex_rest.writemode=false;
    }
    if(knob2){
        unsigned int wb_result = 0;
        if (ma_wb_rest.control.isWb)
        {
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
                wb_result = ma_wb_rest.PC + 4;
            }
            else
            {
                cout << "error :undefined wbSignal" << endl;
            }
        }
        //mux3
        if(forwarding_unit.select_ex_A==0){
            ;
        }
        else if(forwarding_unit.select_ex_A==1){
            de_ex_rest.A=ex_ma_rest.alu_result;
        }
        else if(forwarding_unit.select_ex_A==2){
            de_ex_rest.A=wb_result;
        }
        else{
            cout<<"unidentified select_ex_A"<<endl;
        }
        //mux4
        printf("select_ex_B%d\n",forwarding_unit.select_ex_B);
        printf("ma rd: %d\n",forwarding_unit.ma_inst.rd);//
        printf("ex rs2 : %d",forwarding_unit.ex_inst.rs2);
        if(forwarding_unit.select_ex_B==0){
            ;
        }
        else if(forwarding_unit.select_ex_B==1){
            // printf("%d\n",ex_ma_rest.alu_result);//
            de_ex_rest.B=ex_ma_rest.alu_result;
        }
        else if(forwarding_unit.select_ex_B==2){
            de_ex_rest.B=wb_result;
        }
        else{
            cout<<"unidentified select_ex_A"<<endl;
        }
        //mux5;
        if(forwarding_unit.select_ex_op2==0){
            ;
        }
        else if(forwarding_unit.select_ex_op2==1){
            de_ex_rest.op2=wb_result;
        }
        else{
            cout<<"unidentified select_ex_A"<<endl;
        }

    }
    
    cout<<"\nEXECUTE STAGE"<<endl;
    long long int alu_result;
    alu_result=alu_unit(de_ex_rest.control.aluSignal);
    // printf("%d alu_result\n",alu_result);//
    if(de_ex_rest.control.branchSelect==0){
        //not jalr type
        branchPC=de_ex_rest.branch_target+de_ex_rest.PC;
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
        temp_ex_ma_rest.alu_result=alu_result+de_ex_rest.PC;
    }
    else{
        temp_ex_ma_rest.alu_result=alu_result;
    }
    temp_ex_ma_rest.instruction=de_ex_rest.instruction;
    temp_ex_ma_rest.op2=(unsigned int) de_ex_rest.op2;
    temp_ex_ma_rest.rd=(unsigned int) de_ex_rest.rd;
    temp_ex_ma_rest.rs1=de_ex_rest.rs1;
    temp_ex_ma_rest.rs2=de_ex_rest.rs2;
    temp_ex_ma_rest.PC=de_ex_rest.PC;
    temp_ex_ma_rest.control=de_ex_rest.control;
    if(knob2){
        if((forwarding_unit.ma_inst.opcode=="lb"
        ||forwarding_unit.ma_inst.opcode=="lh"
        ||forwarding_unit.ma_inst.opcode=="lw")){
            if(forwarding_unit.ex_inst.opcode=="sb"
            ||forwarding_unit.ex_inst.opcode=="sh"
            ||forwarding_unit.ex_inst.opcode=="sw"){
                if(forwarding_unit.ifDependencyrs1(forwarding_unit.ex_inst,forwarding_unit.ma_inst)){
                    //stall
                    PCWrite=false;
                    if_de_rest.writemode=false;
                    de_ex_rest.writemode=false;
                    temp_ex_ma_rest.instruction="00000000000000000000000000000000";
                    temp_ex_ma_rest.PC=0;
                    temp_ex_ma_rest.control.set_instruction(temp_ex_ma_rest.instruction);
                    temp_ex_ma_rest.control.build_control();
                }

            }
            else if(forwarding_unit.ifDependencyrs1(forwarding_unit.ex_inst,forwarding_unit.ma_inst)
            ||forwarding_unit.ifDependencyrs2(forwarding_unit.ex_inst,forwarding_unit.ma_inst)){
                //stall
                PCWrite=false;
                if_de_rest.writemode=false;
                de_ex_rest.writemode=false;
                temp_ex_ma_rest.instruction="00000000000000000000000000000000";
                temp_ex_ma_rest.PC=0;
                temp_ex_ma_rest.control.set_instruction(temp_ex_ma_rest.instruction);
                temp_ex_ma_rest.control.build_control();
            }

        }     
    }
    printf("alu result :%u \n",temp_ex_ma_rest.alu_result);//
    printf("op2 : %u\n",temp_ex_ma_rest.op2);//
    printf("rd :%u\n",temp_ex_ma_rest.rd);//
    printf("isBranchTaken %d\n",de_ex_rest.control.isBranchTaken);
    printf("Branch PC(in hex) = %x\n",branchPC);
}

// //perform the memory operation
void mA(bool knob2) {
    if(ex_ma_rest.control.isexit){
        ex_ma_rest.writemode=false;
    }
    if(knob2){
        //forwarding
        unsigned int wb_result = 0;
        if (ma_wb_rest.control.isWb)
        {
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
                wb_result = ma_wb_rest.PC + 4;
            }
            else
            {
                cout << "error :undefined wbSignal" << endl;
            }
        }
        printf("!@# wb_result: %d\n",wb_result);
        //mux6
        if(forwarding_unit.select_ma_op2==0){
            ;
        }
        else if(forwarding_unit.select_ma_op2==1){
            ex_ma_rest.op2=wb_result;
        }
        else{
            cout<<"unidentified select_ma_op2"<<endl;
        }
    }
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
        printf("ex_ma_rest.control.nBytes: %d\n",ex_ma_rest.control.nBytes);
        printf("ex_ma_rest.op2: %d\n",ex_ma_rest.op2);
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
        printf(" value written %d\n",memory_read((unsigned int)ex_ma_rest.alu_result,4));   
        printf("value of alu_result: %x\n",ex_ma_rest.alu_result);
    }
    temp_ma_wb_rest.instruction=ex_ma_rest.instruction;
    temp_ma_wb_rest.alu_result=ex_ma_rest.alu_result;
    temp_ma_wb_rest.ld_result=ldResult;
    temp_ma_wb_rest.rd=ex_ma_rest.rd;
    temp_ma_wb_rest.rs1=ex_ma_rest.rs1;
    temp_ma_wb_rest.rs2=ex_ma_rest.rs2;
    temp_ma_wb_rest.PC=ex_ma_rest.PC;
    temp_ma_wb_rest.control=ex_ma_rest.control;
    cout<<"LdResult :"<<temp_ma_wb_rest.ld_result<<endl;
    cout<<"rd :"<<temp_ma_wb_rest.rd<<endl;
}

// //writes the results back to register file
void write_back()
{   
    if(ma_wb_rest.control.isexit){
        EXIT=true;
        ma_wb_rest.writemode=false;
    }
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
            wb_result = ma_wb_rest.PC + 4;
        }
        else
        {
            cout << "error :undefined wbSignal" << endl;
        }
        registerFile.set_register(ma_wb_rest.rd, wb_result);
        cout << "rd: " << ma_wb_rest.rd << "\nvalue: " << wb_result << endl;
    }
}

void positive_edge_trigger(){
    if((de_ex_rest.control.branchSignal!="nbr")&&(iscorrect_execute())){
        if(de_ex_rest.control.isBranchTaken){
            if(if_de_rest.PC!=branchPC){ // if prediction was false
                //if_de and de_ex set to nop
                printf("** test 34\n");
                temp_if_de_rest.instruction="00000000000000000000000000000000";
                temp_if_de_rest.PC=0;

                temp_de_ex_rest.A=0;temp_de_ex_rest.B=0;temp_de_ex_rest.branch_target=0;
                temp_de_ex_rest.control.set_instruction("00000000000000000000000000000000");
                temp_de_ex_rest.control.build_control();
                temp_de_ex_rest.op2=0;temp_de_ex_rest.PC=0;temp_de_ex_rest.rd=0;

                // //to avoid data depedency and branch conflict 
                PCWrite=true;
                if_de_rest.writemode=true;
                de_ex_rest.writemode=true;
                //ma is always written because never writemode is false;

                //update BTB
                {
                    unsigned int ind=BTB_hash(de_ex_rest.PC);
                    BTB[ind].address=de_ex_rest.PC;
                    BTB[ind].branchPC=branchPC;
                    BTB[ind].branch_taken=true;
                }

                nextPC=branchPC;
                
            }
        }
        else{
            if(if_de_rest.PC!=de_ex_rest.PC+4){
                //if_de and de_ex set to nop
                temp_if_de_rest.instruction="00000000000000000000000000000000";
                temp_if_de_rest.PC=0;

                temp_de_ex_rest.A=0;temp_de_ex_rest.B=0;temp_de_ex_rest.branch_target=0;
                temp_de_ex_rest.control.set_instruction("00000000000000000000000000000000");
                temp_de_ex_rest.control.build_control();
                temp_de_ex_rest.op2=0;temp_de_ex_rest.PC=0;temp_de_ex_rest.rd=0;

                //to avoid data depedency and branch conflict 
                PCWrite=true;
                if_de_rest.writemode=true;
                de_ex_rest.writemode=true;

                //update BTB
                {
                    unsigned int ind=BTB_hash(de_ex_rest.PC);
                    BTB[ind].address=de_ex_rest.PC;
                    BTB[ind].branchPC=branchPC;
                    BTB[ind].branch_taken=false;
                }

                nextPC=de_ex_rest.PC+4;
            }
        }
    }
    if(PCWrite){
        PC=nextPC;
    }
    if(BTB[BTB_hash(PC)].address==PC){
        if(BTB[BTB_hash(PC)].branch_taken){
            nextPC=BTB[BTB_hash(PC)].branchPC;
        }
        else{
            nextPC=PC+4;
        }
    }
    else{
        nextPC=PC+4;
    }
    if(if_de_rest.writemode){
        if_de_rest=temp_if_de_rest;
    }
    if(de_ex_rest.writemode){
        de_ex_rest=temp_de_ex_rest;
    }
    if(ex_ma_rest.writemode){
        ex_ma_rest=temp_ex_ma_rest;
    }
    if(ma_wb_rest.writemode){
        ma_wb_rest=temp_ma_wb_rest;
    }

    PCWrite=true;
    if_de_rest.writemode=true;
    de_ex_rest.writemode=true;
    ex_ma_rest.writemode=true;
    ma_wb_rest.writemode=true;
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
bool iscorrect_execute(){
    if((forwarding_unit.ma_inst.opcode=="lb"
        ||forwarding_unit.ma_inst.opcode=="lh"
        ||forwarding_unit.ma_inst.opcode=="lw")){
            if((forwarding_unit.ex_inst.opcode=="sb"
            ||forwarding_unit.ex_inst.opcode=="sh"
            ||forwarding_unit.ex_inst.opcode=="sw")){
                if(forwarding_unit.ifDependencyrs1(forwarding_unit.ex_inst,forwarding_unit.ma_inst)){
                    return false;
                }
            }
            else if(forwarding_unit.ifDependencyrs1(forwarding_unit.ex_inst,forwarding_unit.ma_inst)
            ||forwarding_unit.ifDependencyrs2(forwarding_unit.ex_inst,forwarding_unit.ma_inst)){
                return false;
            }
    }
    return true;

}

void print_pipeline_register(){
    printf("IF_DE Register\n");
    // printf("",if_de_rest.instruction)

}

