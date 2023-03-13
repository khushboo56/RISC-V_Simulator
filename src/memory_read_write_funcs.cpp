#include "builtin_funcs.hpp"
#include "global_variables.hpp"
void memory_write(unsigned int address, unsigned long long int value, int bytes)
{
    for (int i = 0; i < bytes; i++)
    {
        mem[address + i] = (char)((value >> (i * 8)) & 0xFF);
    }
}
unsigned long long int memory_read(unsigned int address, int bytes)
{
    unsigned long long int value = 0;
    for (int i = 0; i < bytes; i++)
    {
        value = (((unsigned long long int)((unsigned char)mem[address + i])) << (i * 8)) | (value);
    }
    return value;
}