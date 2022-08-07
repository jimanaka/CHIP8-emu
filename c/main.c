#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

int initialize_game(char* game_name) {

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    perror("no game supplied\n");
    exit(1);
  }
  const char* game_name = argv[1];

  // ----- Initialize system and load game -----

  // Memory
  /*
    4KB
    0x000-0x1FF Reserved for CHIP-8 interpreter
    0x050-0x0A0 built in 4x5 pixel font set (0-F)
    0x200-0xFFF program ROM and RAM
  */
  unsigned char memory[4096];

  // Stack
  unsigned short stack[16];

  // Keypad state
  unsigned char key[16];

  // Create cpu
  CPU* cpu = malloc(sizeof(CPU));
  bzero(cpu, sizeof(CPU));

  // ----- Emulation loop -----

  // ----- Clean up -----
  free(cpu);
}
