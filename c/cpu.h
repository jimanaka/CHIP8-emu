typedef struct{
  // general registers: V[0-14], V[15] = Carry register
  unsigned char V[16];

  // Index and program counter
  unsigned short I;
  unsigned short pc;

  // Interupts and hardware timers
  /* 60Hz timers */
  unsigned char delay_timer;
  unsigned char sound_timer;

  // stack pointer
  unsigned short sp;
} CPU;
