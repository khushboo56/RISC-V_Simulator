#include "builtin_funcs.h"
class RegisterFile {
    vector<int> x;
public:
    RegisterFile();
    void print_registers() ;
    void write_back(int rd, int content);
    int get_register(int index);
    void set_register(int index, int data);
};