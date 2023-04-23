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
    int select_de_op1;
    int select_de_op2;
    int select_ex_A;
    int select_ex_B;
    int select_ex_op2;
    int select_ma_op2;
    Forwarding_unit();
    void build_mux_selectors();
    bool ifDependencyrs1(struct forwarding_unit_instruction inst1,struct forwarding_unit_instruction inst2 );
    bool ifDependencyrs2(struct forwarding_unit_instruction inst1,struct forwarding_unit_instruction inst2);
};