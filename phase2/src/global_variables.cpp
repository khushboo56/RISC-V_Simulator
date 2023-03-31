#include"builtin_funcs.hpp"
#include"self_defined_funcs.hpp"
#include "global_variables.hpp"
#ifndef MYCLASSES
#define MYCLASSES
#include "registerfile.hpp"
#include "control_unit.hpp"
#include "forwarding_unit.hpp"
#endif
unsigned int PC=0;
unsigned int nextPC=0;
bool PCWrite;
int branchPC=0;
bool EXIT=false;
unordered_map<unsigned int,unsigned int> mem;
RegisterFile registerFile;
Forwarding_unit forwarding_unit;

// Control_unit mycontrol_unit;

// static unsigned int instruction_word;
// static unsigned int operand1;
// static unsigned int operand2;

struct BTB_entry{
    unsigned int address;
    bool branch_taken;
    unsigned int branchPC;
};
struct BTB_entry * BTB = (struct BTB_entry *)malloc(BTB_SIZE*sizeof(struct BTB_entry));
struct IF_DE_rest{
  string instruction;
  Control_unit new_control;
  unsigned int PC;
  bool writemode;
} if_de_rest,temp_if_de_rest;

struct DE_EX_rest{
    string instruction;
    int branch_target;
    int B;
    int A;
    int op2;
    int rd;
    int rs1;
    int rs2;
    Control_unit control;
    unsigned int PC;
    bool writemode;
}de_ex_rest,temp_de_ex_rest;

struct EX_MA_rest{
    string instruction;
    int alu_result;
    unsigned int op2;
    unsigned int rd;
    int rs1;
    int rs2;
    Control_unit control;
    unsigned int PC;
    bool writemode;
}ex_ma_rest,temp_ex_ma_rest;

struct MA_WB_rest{
    string instruction;
    unsigned int alu_result;
    unsigned int ld_result;
    unsigned int rd;
    int rs1;
    int rs2;
    Control_unit control;
    unsigned int PC;
    bool writemode;
}ma_wb_rest,temp_ma_wb_rest;