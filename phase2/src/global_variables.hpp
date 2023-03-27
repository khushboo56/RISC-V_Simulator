#include"builtin_funcs.hpp"
#include"self_defined_funcs.hpp"
#ifndef MYCLASSES
#define MYCLASSES
#include "registerfile.hpp"
#include "control_unit.hpp"
#endif
#define BTB_SIZE 1024
extern unsigned int PC;
extern unsigned int nextPC;
extern int branchPC;
extern bool EXIT;
extern unordered_map<unsigned int,unsigned int> mem;
extern RegisterFile registerFile;

struct BTB_entry{
    unsigned int address;
    bool branch_taken;
    unsigned int branchPC;
};
extern struct BTB_entry * BTB;
// extern Control_unit mycontrol_unit;

// static unsigned int instruction_word;
// static unsigned int operand1;
// static unsigned int operand2;
struct IF_DE_rest{
  string instruction;
  Control_unit new_control;
  unsigned int PC;
};
struct DE_EX_rest{
    int branch_target;
    int B;
    int A;
    int op2;
    int rd;
    Control_unit control; 
    unsigned int PC;
};

struct EX_MA_rest{
    int alu_result;
    unsigned int op2;
    unsigned int rd;
    Control_unit control; 
    unsigned int PC;
};
struct MA_WB_rest{
    unsigned int alu_result;
    unsigned int ld_result;
    unsigned int rd;
    Control_unit control;
    unsigned int PC; 
};
extern struct IF_DE_rest if_de_rest;
extern struct DE_EX_rest de_ex_rest;
extern struct EX_MA_rest ex_ma_rest;
extern struct MA_WB_rest ma_wb_rest;
extern struct IF_DE_rest temp_if_de_rest;
extern struct DE_EX_rest temp_de_ex_rest;
extern struct EX_MA_rest temp_ex_ma_rest;
extern struct MA_WB_rest temp_ma_wb_rest;
