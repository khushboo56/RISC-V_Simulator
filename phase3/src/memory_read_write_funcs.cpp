#include "builtin_funcs.hpp"
#include "global_variables.hpp"
void memory_write(unsigned int address, unsigned long long int value, int bytes)
{
    if(address>=268439552){
        data_cache.writeCache(address,value,bytes);
        n_accessd++;
    }
    else{
        instruction_cache.writeCache(address,value,bytes);
        n_accessi++;
    }
}
unsigned long long int memory_read(unsigned int address, int bytes)
{
    if(address>=268439552){
        //0x10001000 for data_segment
        n_accessd++;
        return data_cache.readCache(address,bytes);
    }
    else{
        n_accessi++;
        return instruction_cache.readCache(address,bytes);
    }
}
void brut_memory_write(unsigned int address, unsigned long long int value, int bytes)
{
    for (int i = 0; i < bytes; i++)
    {
        mem[address + i] = (char)((value >> (i * 8)) & 0xFF);
    }
}