#include"builtin_funcs.h"
#include"self_defined_funcs_and_classes.h"
#ifndef MYCLASSES
#define MYCLASSES
#include "RegisterFile.h"
#include "Control_unit.h"
#endif
unsigned int PC=0;
unsigned int nextPC=0;
int branchPC=0;
bool EXIT=false;
map<unsigned int,char> mem;
RegisterFile registerFile;
Control_unit mycontrol_unit;

// static unsigned int instruction_word;
// static unsigned int operand1;
// static unsigned int operand2;

struct IF_DE_rest{
  string instruction;
} if_de_rest;

struct DE_EX_rest{
    int branch_target;
    int B;
    int A;
    int op2;
    int rd;
}de_ex_rest;

struct EX_MA_rest{
    int alu_result;
    unsigned int op2;
    unsigned int rd;
}ex_ma_rest;

struct MA_WB_rest{
    unsigned int alu_result;
    unsigned int ld_result;
    unsigned int rd;
}ma_wb_rest;