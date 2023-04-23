#include "builtin_funcs.hpp"
#include "global_variables.hpp"

unsigned int BTB_hash(unsigned int k){
    unsigned int s=2654435731; // (sqrt (5) – 1) / 2 * (2^32)
    unsigned int temp;
    unsigned int p=log2(BTB_SIZE);
    temp=s*k;
    temp=(temp>>(32-p));
    return temp;
}