#include "builtin_funcs.hpp"
//ingnored auipc
#include "control_unit.hpp"

Control_unit::Control_unit(){
    instruction= "00000000000000000000000000000000";
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

        //value of nBytes for load and store 
        if(opcode=="0000011"||opcode=="0100011"){
            string funct3=instruction.substr(17,3);
            if(funct3=="000"){
                nBytes=1;         //lb or sb 
            }
            else if(funct3=="001"){
                nBytes=2;           //lh or sh 
            }
            else if(funct3=="010"){
                nBytes=4;                //lw or sw
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
            wbSignal="pc+4";               //for jal or jalr 
        }
        else{
            wbSignal="alu";
        }
    }    
}
void Control_unit::setIsBranchTaken(bool val){
    isBranchTaken=val;
}
