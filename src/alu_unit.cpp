#include "builtin_funcs.h"
#include "global_variables.h"
int alu_unit(string aluSignal){
    int alu_result;
    if(aluSignal=="add"){
        alu_result=de_ex_rest.A+de_ex_rest.B;
    }
    else if(aluSignal=="sub"){
        alu_result=de_ex_rest.A-de_ex_rest.B;
    }
    else if(aluSignal=="xor"){
        alu_result=de_ex_rest.A^de_ex_rest.B;
    }
    else if(aluSignal=="or"){
        alu_result=de_ex_rest.A|de_ex_rest.B;
    }
    else if(mycontrol_unit.aluSignal=="and"){
        alu_result=de_ex_rest.A&de_ex_rest.B;
    }
    else if(aluSignal=="sll"){
        alu_result=de_ex_rest.A<<de_ex_rest.B;
    }
    else if(aluSignal=="srl"){
        alu_result=(unsigned int)de_ex_rest.A>>(unsigned int)de_ex_rest.B;
    }
    else if(aluSignal=="sra"){
        alu_result=de_ex_rest.A>>de_ex_rest.B;
    }
    else if(aluSignal=="slt"){
        alu_result=(de_ex_rest.A<de_ex_rest.B)?1:0;
    }
    else if(aluSignal=="sltu"){
        alu_result=((unsigned int) de_ex_rest.A<(unsigned int)de_ex_rest.B)?1:0;
    }
    else if(aluSignal=="no_arth"){
        alu_result=de_ex_rest.B;
    }
    return alu_result;
}