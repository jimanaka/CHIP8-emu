#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdbool.h>

#include "cpu.h"

#define MEMSIZE 4096
#define STACKSIZE 16
#define KEYSIZE 16

int initialize(CPU* cpu, unsigned char* memory, unsigned short* stack, unsigned char* key) {
  bzero(cpu, sizeof(CPU));
  bzero(memory, MEMSIZE);
  bzero(stack, STACKSIZE);
  bzero(key, KEYSIZE);

  // set first pc to 0x200
  cpu->pc = 0x200;

  return 0;
}

int initialize_memory(unsigned char* memory, unsigned char* fontset, char* game_name) {
  size_t filesize;
  FILE* gamefp;

  for (int i = 0; i < 80; i++)
    {
      memory[i] = fontset[i];
    }
  printf("fontset loaded\n");

  printf("%s\n", game_name);
  gamefp = fopen(game_name, "r");

  if (gamefp == NULL) {
    perror("File not found\n");
    exit(EXIT_FAILURE);
  }


  fseek(gamefp, 0L, SEEK_END);
  filesize = ftell(gamefp);
  rewind(gamefp);

  unsigned char temp;

  for (int i = 0; i < filesize; i++)
    {
      temp = fgetc(gamefp);
      memory[i + 0x200] = temp;
    }

  return 0;
}

int emulate(CPU* cpu, unsigned char* memory, unsigned short* stack, unsigned char* key) {

  bool loop_game = true;
  u_int8_t opcode;

  while (loop_game)
    {
      // Fetch
      opcode = memory[cpu->pc];

      // Decode

      switch (opcode)
        {
            
        }


      // Execute
    }

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    perror("no game supplied\n");
    exit(1);
  }
  const char* game_name = argv[1];

  // ----- Initialize system and load game -----

  // System fontset
  // From: https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
  unsigned char chip8_fontset[80] =
    { 
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F

    };

  // Memory
  /*
    4KB
    0x000-0x1FF Reserved for CHIP-8 interpreter
    0x050-0x0A0 built in 4x5 pixel font set (0-F)
    0x200-0xFFF program ROM and RAM
  */
  unsigned char memory[MEMSIZE];

  // Stack
  unsigned short stack[STACKSIZE];

  // Keypad state
  unsigned char key[KEYSIZE];

  // Create cpu
  CPU* cpu = malloc(sizeof(CPU));

  // Initialize cpu
  initialize(cpu, memory, stack, key);

  // Load data into memory
  initialize_memory(memory, chip8_fontset, (char*) game_name);

  // ----- Emulation loop -----

  emulate(cpu, memory, stack, key);

  // ----- Clean up -----
  free(cpu);
}
