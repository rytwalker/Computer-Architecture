#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

// cpu_ram_read
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  // address == MAR
  return cpu->ram[address];
}

// cpu_ram_write
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  // address == MAR value == MDR (Memory Data Register)
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fptr;
  char line[1024];
  unsigned char address = 0;
  fptr = fopen(file, "r");

  if (fptr == NULL)
  {
    fprintf(stderr, "error opening %s\n", file);
    exit(2);
  }

  while (fgets(line, sizeof(line), fptr) != NULL)
  {
    unsigned char b;
    b = strtoul(line, NULL, 2);
    cpu_ram_write(cpu, address++, b);
  }

  fclose(fptr);
}

/**
 * ALU
 */
/*
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}
*/

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR;
  unsigned char operandA;
  unsigned char operandB;
  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction

    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case LDI:
      // cpu_ram_write(cpu, operandA, operandB);
      cpu->registers[operandA] = operandB;
      cpu->PC += 3;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      cpu->PC += 2;
      break;
    case MUL:
      cpu->registers[operandA] = cpu->registers[0] * cpu->registers[1];
      cpu->PC += 3;
      break;
    case HLT:
      running = 0;
      break;
    default:
      printf("unexpected instruction 0x%02X at 0x%02X", IR, cpu->PC);
      exit(1);
      break;
    }

    // IR = cpu_ram_read(cpu, cpu->PC);
    // 6. Move the PC to the next instruction.
  }
  // printf("%d\n", IR);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // INIT REGISTERS
  memset(cpu->registers, 0, 7);
  cpu->registers[7] = 0xf4;
  // INIT PC
  cpu->PC = 0;
  // INIT RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
