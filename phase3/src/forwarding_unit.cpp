#include "forwarding_unit.hpp"
Forwarding_unit::Forwarding_unit(){


}
void Forwarding_unit::build_mux_selectors(){
    //val for de_op1 first mux
    if(ifDependencyrs1(de_inst,wb_inst)){
        select_de_op1=1;
    }
    else{
        select_de_op1=0;
    }

    //val for de_op2 second mux
    if(ifDependencyrs2(de_inst,wb_inst)){
        select_de_op2=1;
    }
    else{
        select_de_op2=0;
    }

    //val for ex_A third mux
    if(ifDependencyrs1(ex_inst,ma_inst)){
        //according to refernce diagram 
        //ma frowarding given prefernce over wb forwarding
        select_ex_A=1;
    }
    else if(ifDependencyrs1(ex_inst,wb_inst)){
        //according to refernce diagram
        select_ex_A=2;
    }
    else{
        //according to refernce diagram
        select_ex_A=0;
    }

    //selector for ex_B fourth mux
    if(ex_inst.opcode=="sb"||ex_inst.opcode=="sh"||ex_inst.opcode=="sw"){
        select_ex_B=0;
    }
    else if(ifDependencyrs2(ex_inst,ma_inst)){
        //according to refernce diagram 
        //ma frowarding given prefernce over wb forwarding
        select_ex_B=1;
    }
    else if(ifDependencyrs2(ex_inst,wb_inst)){
        //according to refernce diagram
        select_ex_B=2;
    }
    else{
        //according to refernce diagram
        select_ex_B=0;
    }

    //selector for ex_op2 fifth mux
    if(ifDependencyrs2(ex_inst,wb_inst)){
        select_ex_op2=1;
    }
    else{
        select_ex_op2=0;
    }

    //selector for ma_op2 sixth mux
    if(ifDependencyrs2(ma_inst,wb_inst)){
        select_ma_op2=1;
    }
    else{
        select_ma_op2=0;
    }
}
bool Forwarding_unit::ifDependencyrs1(struct forwarding_unit_instruction inst1,struct forwarding_unit_instruction inst2){
    if(inst1.rs1==0||inst2.rd==0){
        return false;
    }
    if((inst1.opcode=="exit")||(inst1.opcode=="nop")||(inst1.opcode=="jal")||(inst1.opcode=="lui")||(inst1.opcode=="auipc")){
        //inst1 does not read from register
        return false;
    }
    if((inst2.opcode=="exit")||inst2.opcode=="nop"||inst2.opcode=="sb"||inst2.opcode=="sh"||
    inst2.opcode=="sw"||inst2.opcode=="beq"||inst2.opcode=="bne"||
    inst2.opcode=="blt"||inst2.opcode=="bge"){
        //inst2 doesnt write to register file
        return false;
    }
    if(inst1.rs1==inst2.rd){
        return true;
    }
    return false;
}
bool Forwarding_unit::ifDependencyrs2(struct forwarding_unit_instruction inst1,struct forwarding_unit_instruction inst2){
    if(inst1.rs2==0||inst2.rd==0){
        return false;
    }
    if((inst1.opcode=="exit")||(inst1.opcode=="nop")||inst1.opcode=="addi"||inst1.opcode=="xori"||inst1.opcode=="ori"||inst1.opcode=="andi"||
    inst1.opcode=="slli"||inst1.opcode=="srli"||inst1.opcode=="srai"||inst1.opcode=="slti"||
    inst1.opcode=="lb"||inst1.opcode=="lh"||inst1.opcode=="lw"||
    (inst1.opcode=="jal")||inst1.opcode=="jalr"||(inst1.opcode=="lui")||(inst1.opcode=="auipc")){
        //inst1 does not read from register file
        return false;
    }
    if(inst2.opcode=="exit"||inst2.opcode=="nop"||inst2.opcode=="sb"||inst2.opcode=="sh"||
    inst2.opcode=="sw"||inst2.opcode=="beq"||inst2.opcode=="bne"||
    inst2.opcode=="blt"||inst2.opcode=="bge"){
        //inst2 doesnt write to register file
        return false;
    }
    if(inst1.rs2==inst2.rd){
        return true;
    }
    return false;
}
