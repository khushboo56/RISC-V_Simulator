#include "builtin_funcs.hpp"

long long int immediate(string instruction);
string dec2bin(unsigned int n);
long long int unsgn_binaryToDecimal(string n);
long long int bin2dec(string str);
long long int unsgn_binaryToDecimal(string n);
long long int sign_extn(string bin);
long long int unsign_extn(string bin);
int alu_unit(string aluSignal);
void memory_write(unsigned int address,unsigned long long int value,int bytes);
unsigned long long int memory_read(unsigned int address,int bytes);