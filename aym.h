#define BC1 10
#define BDIR 11
#define RESET 12
#define CLOCK 13

// #define SEL 11

void set_mode_inactive();
void set_mode_latch();
void set_mode_write();
void set_mode_read();

void write_register(char reg, char value);