#include "builtin_funcs.hpp"
struct forwarding_unit_instruction{
    string opcode;
    int rd;
    int rs1;
    int rs2;
};

class Forwarding_unit{
    public:
    struct forwarding_unit_instruction de_inst;
    struct forwarding_unit_instruction ex_inst;
    struct forwarding_unit_instruction ma_inst;
    struct forwarding_unit_instruction wb_inst;
    int de_op1;
    int de_op2;
    int ex_A;
    int ex_B;
    int ex_op2;
    int ma_op2;
    Forwarding_unit();
    void build_mux_selectors();
    bool ifDependencyrs1(struct forwarding_unit_instruction inst1,struct forwarding_unit_instruction inst2 );
    bool ifDependencyrs2(struct forwarding_unit_instruction inst1,struct forwarding_unit_instruction inst2);
};