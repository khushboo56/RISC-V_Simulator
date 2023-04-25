#include"builtin_funcs.hpp"
#include"self_defined_funcs.hpp"
#include "global_variables.hpp"
#ifndef MYCLASSES
#define MYCLASSES
#include "registerfile.hpp"
#include "control_unit.hpp"
#include "forwarding_unit.hpp"
#include "cache.hpp"
#endif
unsigned int PC=0;
unsigned int nextPC=0;
bool PCWrite;
int branchPC=0;
bool EXIT=false;

unordered_map<unsigned int,unsigned int> mem;
Control_unit without_pipeline_control_unit;
RegisterFile registerFile;
Forwarding_unit forwarding_unit;
Cache instruction_cache;
Cache data_cache; 

// Control_unit mycontrol_unit;

// static unsigned int instruction_word;
// static unsigned int operand1;
// static unsigned int operand2;


struct BTB_entry * BTB = (struct BTB_entry *)malloc(BTB_SIZE*sizeof(struct BTB_entry));
struct IF_DE_rest if_de_rest,temp_if_de_rest;

struct DE_EX_rest de_ex_rest,temp_de_ex_rest;

struct EX_MA_rest ex_ma_rest,temp_ex_ma_rest;

struct MA_WB_rest ma_wb_rest,temp_ma_wb_rest;

//
bool knob3=false;
bool knob4=false;

//stats
FILE * sp_out;
FILE * stats;
long long int check_inst=0;
int n_cycles=0,n_instruct=0,n_data_transfer=0,n_ALU_instruct=0;
double CPI=0;
int n_control_instruct=0,n_stalls=0,n_control_hazards=0;
int n_branch_mispredicts=0,n_stalls_data=0,n_stalls_control=0;