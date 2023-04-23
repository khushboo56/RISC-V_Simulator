void fetch();
void decode(bool knob2);
void execute(bool knob2);
void mA(bool knob2);
void write_back();
void display();
void positive_edge_trigger();
void run_riscvsim(bool knob2);
void reset_proc();
void load_program_memory(char *file_name);
bool iscorrect_execute();
void print_pipeline_register();

//whithout pipeline

void fetch_without_pipeline();
void decode_without_pipeline();
void execute_without_pipeline();
void mA_without_pipeline();
void write_back_without_pipeline();
void display_without_pipeline();
void run_riscvsim_without_pipeline();