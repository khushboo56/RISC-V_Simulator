#include "builtin_funcs.hpp"
class Control_unit{
    public:
    string instruction;
    bool isImmediate;
    string aluSignal;
    string branchSignal;
    bool isBranchTaken;
    bool isexit;
    int branchSelect;
    bool isLd;
    bool isSt;
    bool isWb;
    int nBytes;
    bool isauipc;
    string wbSignal;
    Control_unit();
    void set_instruction(string instruct);
    void build_control();
    void setIsBranchTaken(bool val);
};