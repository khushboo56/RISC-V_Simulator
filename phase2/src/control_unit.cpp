#include "builtin_funcs.hpp"
#include "control_unit.hpp"

Control_unit::Control_unit(){
    instruction= "00000000000000000000000000000000";
    inst_type="undef";
    isImmediate=false;
    aluSignal="add";
    branchSignal="nbr";
    isBranchTaken=false; //intially set to false later will be updated in execute stage by branch unit
    branchSelect=0;
    isLd=false;
    isSt=false;
    isWb=false;
    wbSignal="alu";
    isauipc=false;
    nBytes=0;
    isexit=false;

}
void Control_unit::set_instruction(string instruct){
    instruction=instruct;
}
void Control_unit::build_control(){
    string opcode=instruction.substr(25,7);
    
    if(opcode=="1111111"){
        isexit=true;
    }
    else{
        isexit=false;
    }
    //value of inst_type;
    if(opcode=="0110011"){
        string funct3=instruction.substr(17,3);
        string funct7=instruction.substr(0,7);
        if(funct3=="000"){
            if(funct7=="0000000"){
                inst_type="add";
            }
            else if(funct7=="0100000"){
                inst_type="sub";
            }
        }
        else if(funct3=="001"){
            inst_type="sll";
        }
        else if(funct3=="010"){
            inst_type="slt";
        }
        else if(funct3=="011"){
            inst_type="sltu";
        }
        else if(funct3=="100"){
            inst_type="xor";
        }
        else if(funct3=="101"){
            if(funct7=="0000000"){
                inst_type="srl";
            }
            else if(funct7=="0100000"){
                inst_type="sra";
            }
        }
        else if(funct3=="110"){
            inst_type="or";
        }
        else if(funct3=="111"){
            inst_type="and";
        }
    }
    else if(opcode=="0010011"){
        string funct3=instruction.substr(17,3);
        if(funct3=="000"){
            inst_type="addi";
        }
        else if(funct3=="001"){
            inst_type="slli";
        }
        else if(funct3=="010"){
            inst_type="slti";
        }
        else if(funct3=="011"){
            inst_type="sltiu";
        }
        else if(funct3=="100"){
            inst_type="xori";
        }
        else if(funct3=="101"){
            string funct7=instruction.substr(0,7);
            if(funct7=="0000000"){
                inst_type="srli";
            }
            else if(funct7=="0100000"){
                inst_type="srai";
            }
        }
        else if(funct3=="110"){
            inst_type="ori";
        }
        else if(funct3=="111"){
            inst_type="andi";
        }
    }
    else if(opcode=="0000011"){
        string funct3=instruction.substr(17,3);
        if(funct3=="000"){
            inst_type="lb";
        }
        else if(funct3=="001"){
            inst_type="lh";
        }
        else if(funct3=="010"){
            inst_type="lw";
        }
        else if(funct3=="100"){
            inst_type="lbu";
        }
        else if(funct3=="101"){
            inst_type="lhu";
        }
    }
    else if(opcode=="0100011"){
        string funct3=instruction.substr(17,3);
        if(funct3=="000"){
            inst_type="sb";
        }
        else if(funct3=="001"){
            inst_type="sh";
        }
        else if(funct3=="010"){
            inst_type="sw";
        }
    }
    else if(opcode=="1100011"){
        string funct3=instruction.substr(17,3);
        if(funct3=="000"){
            inst_type="beq";
        }
        else if(funct3=="001"){
            inst_type="bne";
        }
        else if(funct3=="100"){
            inst_type="blt";
        }
        else if(funct3=="101"){
            inst_type="bge";
        }
        else if(funct3=="110"){
            inst_type="bltu";
        }
        else if(funct3=="111"){
            inst_type="bgeu";
        }

    }
    else if(opcode=="1101111"){
        inst_type="jal";
    }
    else if(opcode=="1100111"){
        inst_type="jalr";
    }
    else if(opcode=="0110111"){
        inst_type="lui";
    }
    else if(opcode=="0010111"){
        inst_type="auipc";
    }
    else if(opcode=="0000000"){
        inst_type="nop";
    }
    else if(opcode=="1111111"){
        inst_type="exit";
    }
    else{
        cout<<"undefined opcode"<<endl;
    }
    
    //value of isImmediate
    if(opcode=="0010011"||opcode=="0000011"||opcode=="0100011"||opcode=="1100111"||opcode=="0110111"||opcode=="0010111"){
        isImmediate=true; //one for arithmetic immediate, load, store, jalr, lui, auipc
    }
    else{
        isImmediate=false;
    }

    //isauipc
    if(opcode=="0010111"){
        isauipc=true;
    }
    else{
        isauipc=false;
    }
    //value for branchSelect
    //jalr branch select is 1;
    if(opcode=="1100111"){
        branchSelect=1;
    }
    else{
        branchSelect=0;
    }

    

    //value of aluSignal
    if(opcode=="0110011"||opcode=="0010011"){
        string funct3=instruction.substr(17,3);
        string funct7=instruction.substr(0,7);
        if(funct3=="000"){
            if(opcode=="0110011"){
                if(funct7=="0000000"){
                    aluSignal="add";
                }
                else if(funct7=="0100000"){
                    aluSignal="sub";
                }
            }
            else if(opcode=="0010011"){
                aluSignal="add";
            }
        }
        else if(funct3=="100"){
            aluSignal="xor";
        }
        else if(funct3=="110"){
            aluSignal="or";
        }
        else if(funct3=="111"){
            aluSignal="and";
        }
        else if(funct3=="001"){
            aluSignal="sll";
        }
        else if(funct3=="101"){
            if(funct7=="0000000"){
                aluSignal="srl";
            }
            else if(funct7=="0100000"){
                aluSignal="sra";
            }   
        }
        else if(funct3=="010"){
            aluSignal="slt";
        }
        else if(funct3=="011"){
            aluSignal="sltu";
        }
    }
    else if(opcode=="0000011"||opcode=="0100011"){
        aluSignal="add";
    }
    //for branch arithmetic operatio is subtraction
    else if(opcode=="1100011"){
        aluSignal="compare";
    }
    //for jalr addition
    else if(opcode=="1100111"){
        aluSignal="add";
    }
    else{
        aluSignal="no_arth";
    }

    //value of branchSignal
    if(opcode=="1100011"){
        // for conditional branch statements 
        string funct3=instruction.substr(17,3);
        if(funct3=="000"){
            branchSignal="beq";
        }
        else if(funct3=="001"){
            branchSignal="bne";
        }
        else if(funct3=="100"){
            branchSignal="blt";
        }
        else if(funct3=="101"){
            branchSignal="bge";
        }
        else if(funct3=="110"){
            branchSignal="bltu";
        }
        else if(funct3=="111"){
            branchSignal="bgeu";
        }
        else{
            cout<<"** Undefined funct3 **"<<endl;
        }
    }
    else if(opcode=="1101111"||opcode=="1100111"){
        //jal and jalr as its unconditional branch
        branchSignal="ubr";
    }
    else{
        branchSignal="nbr";
    }

    //value of isLd
    if(opcode=="0000011"){
        isLd=true;
    }
    else{
        isLd=false;
    }
    
    //value of isSt
    if(opcode=="0100011"){
        isSt=true;
    }
    else{
        isSt=false;
    }

    //value of nBytes
    if(opcode=="0000011"||opcode=="0100011"){
        string funct3=instruction.substr(17,3);
        if(funct3=="000"){
            nBytes=1;
        }
        else if(funct3=="001"){
            nBytes=2;
        }
        else if(funct3=="010"){
            nBytes=4;
        }
    }
    else{
        nBytes=0;
    }
    
    //value of isWb
    if(opcode=="0110011"||opcode=="0010011"||opcode=="0000011"||opcode=="1101111"||
    opcode=="1100111"||opcode=="0110111"||opcode=="0010111"){
        isWb=true;
    }
    else{
        isWb=false;
    }

    //value of wbSignal
    if(opcode=="0110011"||opcode=="0010011"||opcode=="0110111"||opcode=="0010111"){
        wbSignal="alu";
    }
    else if(opcode=="0000011"){
        wbSignal="ld";
    }
    else if(opcode=="1101111"||opcode=="1100111"){
        wbSignal="pc+4";
    }
    else{
        wbSignal="alu";
    }

}
void Control_unit::setIsBranchTaken(bool val){
    isBranchTaken=val;
}
