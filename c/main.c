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

  fclose(gamefp);
  return 0;
}

int emulate(CPU* cpu, unsigned char* memory, unsigned short* stack, unsigned char* key) {

  bool loop_game = true;
  u_int16_t opcode;

  while (loop_game)
    {
      // Fetch
      opcode = memory[cpu->pc] << 8 | memory[cpu->pc + 1];

      // Decode
      // Use first 4 bits to determine action, last 12 to determine other stuff
      switch (opcode & 0xF000)
        {
          case 0x0000:
            switch (opcode & 0x0FFF)
              {
                case 0x0E0:
                  // CLS
                  cpu->pc += 2;
                  break;

                case 0x0EE:
                  // RET
                  cpu->pc += 2;
                  break;

                default:
                  printf("invalid command: [%0x]\n", opcode);
                  exit(EXIT_FAILURE);
              }
            break;

          case 0x1000:
            // JP addr
            cpu->pc = opcode & 0x0FFF;
            break;

          case 0x2000:
            // CALL addr
            stack[cpu->sp] = cpu->pc;           // PUSH RET addr
            cpu->sp++;                          // INC SP
            cpu->pc = opcode & 0x0FFF;           // JMP addr
            break;

          case 0x3000:
            // 0x3xkk
            // Skip next instruction of Vx = kk
            if (cpu->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) cpu->pc += 4;
            else cpu->pc += 2;
            break;

          case 0x4000:
            // 0x4xkk
            // Skip next instruction if Vx != kk
            if (cpu->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) cpu->pc += 4;
            else cpu->pc += 2;
            break;

          case 0x5000:
            // 0x5xy0
            // Skip next instruction if Vx == Vy
            if (cpu->V[(opcode & 0x0F00) >> 8] == cpu->V[(opcode & 0x00F0) >> 4]) cpu->pc += 4;
            else cpu->pc += 2;
            break;

          case 0x6000:
            // 0x6xkk
            // puts value kk into register Vx
            cpu->V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            cpu->pc += 2;
            break;

          case 0x7000:
            // 0x7xkk
            // Adds value kk to the value of register Vx, then store in Vx
            cpu->V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            cpu->pc += 2;
            break;

          case 0x8000:
            switch (opcode & 0x000F)
              {
                case 0x0000:
                  // 0x8xy0
                  // Vx = Vy
                  cpu->V[(opcode & 0x0F00) >> 8] = cpu->V[(opcode & 0x00F0) >> 4];
                  cpu->pc += 2;
                  break;

                case 0x0001:
                  // 0x8xy1
                  // Vx = Vx OR Vy
                  cpu->V[(opcode & 0x0F00) >> 8] |= cpu->V[(opcode & 0x00F0) >> 4];
                  cpu->pc += 2;
                  break;

                default:
                  printf("invalid command: [%0x]\n", opcode);
                  exit(EXIT_FAILURE);
              }
            break;

          default:
            printf("invalid command: [%0x]\n", opcode);
            exit(EXIT_FAILURE);
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
