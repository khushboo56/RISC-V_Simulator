/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developer's Name:
Developer's Email id:
Date: 

*/


/* main.cpp 
   Purpose of this file: The file handles the input and output, and
   invokes the simulator
*/


#include "builtin_funcs.hpp"

#include <stdio.h>
#include <stdlib.h>
#include "myRISCVSim.hpp"
#ifndef GLOBAL
#define GLOBAL
#include "global_variables.hpp"
#endif
int main(int argc, char** argv) {
  int inp;
  char* prog_mem_file;
  bool knob1=false; 
  bool knob2=false;
  // if( freopen( "output.txt", "w", stdout ))
  // {
  //   printf( "this should go to the output file\n" );
  // }
  if(argc < 2) {
    printf("Incorrect number of arguments. Please invoke the simulator \n\t./myRISCVSim <input mem file> \n");
    exit(1);
  }

  //
  printf("PRESS 0 for non_pipeline, PRESS 1 for pipeline: ");
  cin>>inp;
  if(inp==1){
    //stats file
    stats=fopen("statistics.txt","w");
    knob1=true;
    printf("PRESS 0 for NO_Data_forwarding, PRESS 1 for Data_forwarding: ");
    cin>>inp;
    if(inp==1){
      knob2=1;
    }
    else{
      knob2=0;
    }  
  }
  else{
    knob1=false;
  }

  printf("If you want to check for particular instruction PRESS 1 else press 0 \nYour Choice: ");
  cin>>inp;
  if(inp==1){
    int num;
    sp_out=fopen("specific_instruction_output.txt","w");
    knob4=true;
    printf("enter the intruction number :");
    cin>>num;
    check_inst=(num-1)*4; 
  }
  else{
    knob4=false;
    sp_out=NULL;
  }
  printf("Enter the cache size: ");
  scanf(" %d",&CACHESIZE);
  printf("Enter blocksize: ");
  scanf(" %d",&BLOCKSIZE);
  printf("Enter no. of ways: ");
  scanf(" %d",&WAYS);
  printf("Enter replacement policy 0 for LRU 1 for LFU and 2 for FIFO 3 for Random: ");
  scanf(" %d",&REPLACEMENTPOLICY);
  printf("Enter mapping choice: ");
  scanf(" %d",&MAPPINGCHOICE);
  printf("Enter hit time: ");
  scanf(" %d",&hittime);
  printf("Enter miss time: ");
  scanf(" %d",&misstime);

  
  //reset the processor
  reset_proc();
  //load the program memory
  load_program_memory(argv[1]);
  
  //run the simulator
  if(knob1){
    run_riscvsim(knob2);
    fprintf(stats,"Total number of cycles: %d\n",n_cycles);
    fprintf(stats,"Total instruction executed: %d\n",n_instruct);
    CPI=((double)n_cycles)/((double)n_instruct);
    fprintf(stats,"CPI: %lf\n",CPI);
    fprintf(stats,"Total number of data transfers: %d\n",n_data_transfer);
    fprintf(stats,"Total number of ALU instruction executed: %d\n",n_ALU_instruct);
    fprintf(stats,"Total number of control instructions executed: %d\n",n_control_instruct);
    n_stalls=n_stalls_data+n_stalls_control;
    fprintf(stats,"Total number of stalls/bubbles in the pipeline: %d\n",n_stalls);
    fprintf(stats,"Total number of control hazards: %d\n",n_control_hazards);
    fprintf(stats,"Total number of branch misprediction: %d\n",n_branch_mispredicts);
    fprintf(stats,"Total number of stalls due to data hazards: %d\n",n_stalls_data);
    fprintf(stats,"Total number of stalls due to control hazards: %d\n",n_stalls_control);
    FILE* idata=fopen("instruction_cache_stats.txt","w");
    FILE* ddata=fopen("data_cache_stats.txt","w");
    fprintf(idata,"Number of access: %d\n",n_accessi);
    fprintf(idata,"Number of hits: %d\n",n_hitsi);
    fprintf(idata,"Number of miss: %d\n",n_missi);
    fprintf(idata,"Number of cold miss: %d\n",n_coldi);
    fprintf(idata,"Number of conflict and capacity miss: %d\n",n_conflicti);
    fprintf(idata,"Number of memory stalls: %d\n",n_hitsi*(hittime)+n_missi*misstime);
    fprintf(idata,"CPI with perfect cache: %d\n",CPI);
    double withoutCPI=((double)n_cycles+(n_hitsi*(hittime)+n_missi*misstime))/((double)n_instruct);
    fprintf(idata,"CPI without perfect cache: %d\n",withoutCPI);

    fprintf(ddata,"Number of access: %d\n",n_accessd);
    fprintf(ddata,"Number of hits: %d\n",n_hitsd);
    fprintf(ddata,"Number of miss: %d\n",n_missd);
    fprintf(ddata,"Number of cold miss: %d\n",n_coldd);
    fprintf(ddata,"Number of conflict and capacity miss: %d\n",n_conflictd);
    fprintf(ddata,"Number of memory stalls: %d\n",n_hitsd*(hittime)+n_missd*misstime);
    fprintf(ddata,"CPI with perfect cache: %d\n",CPI);
    withoutCPI=((double)n_cycles+(n_hitsd*(hittime)+n_missd*misstime))/((double)n_instruct);
    fprintf(ddata,"CPI without perfect cache: %d\n",withoutCPI);




  }
  else{
    run_riscvsim_without_pipeline();
  }
  fclose(sp_out);
  fclose(stats);

  return 1;
}
