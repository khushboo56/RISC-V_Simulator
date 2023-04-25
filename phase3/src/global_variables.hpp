#include"builtin_funcs.hpp"
#include"self_defined_funcs.hpp"
#ifndef MYCLASSES
#define MYCLASSES
#include "registerfile.hpp"
#include "control_unit.hpp"
#include "forwarding_unit.hpp"
#include "cache.hpp"
#endif
#define BTB_SIZE 1024
extern unsigned int PC;
extern bool PCWrite;
extern unsigned int nextPC;
extern int branchPC;
extern bool EXIT;
extern unordered_map<unsigned int,unsigned int> mem;
extern RegisterFile registerFile;
extern Control_unit without_pipeline_control_unit;
extern Forwarding_unit forwarding_unit;
extern Cache instruction_cache;
extern Cache data_cache; 

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
  bool writemode;
};
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
};

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
};
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
};
extern struct IF_DE_rest if_de_rest;
extern struct DE_EX_rest de_ex_rest;
extern struct EX_MA_rest ex_ma_rest;
extern struct MA_WB_rest ma_wb_rest;
extern struct IF_DE_rest temp_if_de_rest;
extern struct DE_EX_rest temp_de_ex_rest;
extern struct EX_MA_rest temp_ex_ma_rest;
extern struct MA_WB_rest temp_ma_wb_rest;

//knobs
extern bool knob3;
extern bool knob4;

//stats

extern FILE * sp_out;
extern FILE * stats;
extern long long int check_inst;
extern int n_cycles,n_instruct,n_data_transfer,n_ALU_instruct;
extern double CPI;
extern int n_control_instruct,n_stalls,n_control_hazards;
extern int n_branch_mispredicts,n_stalls_data,n_stalls_control;
extern int n_accessd,n_hitsd,n_missd, n_coldd,n_conflictd,n_capacityd;
extern int n_accessi,n_hitsi,n_missi, n_coldi,n_conflicti,n_capacityi;
extern int CACHESIZE, BLOCKSIZE,WAYS,REPLACEMENTPOLICY,MAPPINGCHOICE,hittime,misstime;