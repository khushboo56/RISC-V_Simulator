#include "builtin_funcs.h"
#include"self_defined_funcs_and_classes.h"
#include "RegisterFile.h"
RegisterFile::RegisterFile() {
    for (int i = 0; i < 32; i++) {
        x.push_back(0);
        if (i == 2) {
            // x[i] = 0x7FFFFFF0; // sp
                x[i] = strtol("0x7FFFFFF0", NULL, 16); // sp
            
        }
        else if (i == 3) {
            // x[i] = 0x10000000; // gp
            x[i] = strtol("0x10000000", NULL, 16); // gp
        }
    }
}

void RegisterFile::print_registers() {
    for (int i = 0; i < 32; i++) {
        cout << "x[" << i << "] = " << x[i] << endl;
    }
}

void RegisterFile::write_back(int rd, int content) {
    if (rd != 0) {
        x[rd] = content;
        cout << "WRITEBACK: write " << content << " to x[" << rd << "]" << endl;
    }
    cout << endl;
}

int RegisterFile::get_register(int index) {
    return x[index];
}

void RegisterFile::set_register(int index, int data) {
    if (index != 0) {
        x[index] = data;
    }
}

// int main() {
//     RegisterFile rf;
//    // map<int, int> memory;


//     // Example usage
//     rf.print_registers();
//     rf.write_back(1, 42);
//     rf.set_register(2, 0x80000000);
//     rf.print_registers();
//     int data = 123;
//    // memory[0x100] = data;
//    // cout << "Memory[0x100] = " << memory[0x100] << endl;

//     return 0;
// }
