#include "cpu.h"

u8 ReadNext(CPU *cpu)
{
	return cpu->memory[cpu->registers.PC];
}

u16 ReadNextShort(CPU* cpu)
{
	return cpu->memory[cpu->registers.PC] << 8 | cpu->memory[cpu->registers.PC+1];
}

u16 MemReadShort(CPU* cpu, u16 address)
{
	return (cpu->memory[address] | (cpu->memory[address + 1] << 8));
}

void MemWriteShort(CPU* cpu, u16 addr, u16 val)
{
	cpu->memory[addr] = (val & 0xFF);
	cpu->memory[addr + 1] = (val & 0xFF00) >> 8;
}

void ins_nop(CPU* cpu);
void ins_0x01(CPU *cpu);
void ins_0x02(CPU *cpu);
void ins_0x03(CPU* cpu);
void ins_0x04(CPU* cpu);
void ins_0x05(CPU *cpu);
void ins_0x06(CPU* cpu);
void ins_0x07(CPU* cpu);
void ins_0x08(CPU* cpu);
void ins_0x09(CPU* cpu);
void ins_0x0A(CPU* cpu);
void ins_0x0B(CPU* cpu);
void ins_0x0C(CPU* cpu);

//Do a thing :D
void ins_nop(CPU *cpu)
{
	//do nothing but consume 4 cycles
}
//LD BC, d16
void ins_0x01(CPU* cpu)
{
	cpu->registers.BC = ReadNextShort(cpu);
}

//LD (BC), A
void ins_0x02(CPU* cpu)
{
	cpu->memory[cpu->registers.BC] = cpu->registers.A;
}

//INC BC - no flags
void ins_0x03(CPU *cpu)
{
	cpu->registers.BC++;
}

//INC B - Z 0 H -
void ins_0x04(CPU* cpu)
{
	cpu->registers.B++;
	cpu->registers.negative = 0;

	if (cpu->registers.B)
		cpu->registers.zero = 0;
	else
		cpu->registers.zero = 1;

	if ((cpu->registers.B & 0xF) == 0)
		cpu->registers.halfCarry = 1;
	else
		cpu->registers.halfCarry = 0;
}

//DEC B - Z 1 H -
void ins_0x05(CPU* cpu)
{
	cpu->registers.B--;
	cpu->registers.zero = (cpu->registers.B > 0);
	cpu->registers.halfCarry = ((cpu->registers.B & 0xF) == 0xF);
	cpu->registers.negative = 1;
}

//LD B, d8
void ins_0x06(CPU *cpu)
{
	cpu->registers.B = ReadNext(cpu);
}

//RCLA - 0 0 0 C
void ins_0x07(CPU *cpu)
{
	cpu->registers.carry = cpu->registers.A & 1;

	cpu->registers.A <<= 1;
	cpu->registers.A += cpu->registers.carry;

	cpu->registers.zero = cpu->registers.negative = cpu->registers.halfCarry = 0;
}

//LD (a16), SP
void ins_0x08(CPU* cpu)
{
	MemWriteShort(cpu, ReadNextShort(cpu), cpu->registers.SP);
}

//ADD HL, BC  -  - 0 H C
void ins_0x09(CPU* cpu)
{
	cpu->registers.carry = (cpu->registers.HL + cpu->registers.BC) > 0xFFFF;

	cpu->registers.HL = cpu->registers.BC;

	cpu->registers.halfCarry = (cpu->registers.HL & 0xF + cpu->registers.BC & 0xF) > 0xF;

	cpu->registers.negative = 0;
}

//LD A, (BC)
void ins_0x0A(CPU* cpu)
{
	cpu->registers.A = cpu->memory[cpu->registers.BC];
}

//
void ins_0x0B(CPU* cpu)
{
	cpu->registers.BC--;
}

void ins_0x0C(CPU* cpu)
{
	cpu->registers.C++;
}

void ins_0x0D(CPU* cpu)
{
	cpu->registers.D--;
}

// LD C, d8
void ins_0x0E(CPU* cpu)
{
	cpu->registers.C = ReadNext(cpu);
}

const INSTRUCTION instructions[256] = {
	{"NOP", 1, 4, ins_nop},
	{"LD BC, 0x%04x", 3, 12, ins_0x01},
	{"LD (BC), A", 1, 8, ins_0x02},
	{"INC BC", 1, 8, ins_0x03},
	{"INC B", 1, 4, ins_0x04},
	{"DEC B", 1, 4, ins_0x05},
	{"LD B, 0x%02x", 2, 8, ins_0x06},
	{"RCLA", 1, 4, ins_0x07},
	{"LD (a16), SP", 3, 20, ins_0x08},
	{"ADD HL, BC", 1, 8, ins_0x09},
	{"LD A, BC", 1, 8, ins_0x0A},
	{"DEC BC", 1, 8, ins_0x0B},
	{"INC C", 1, 4, ins_0x0C}
};

#include <stdio.h>
u8 CPU_Execute(CPU* cpu)
{
	const u16 cPC = cpu->registers.PC;
	printf("%d", instructions[cpu->memory[cPC]].length);
	instructions[cpu->memory[cpu->registers.PC++]].exc(cpu);
	cpu->registers.PC += instructions[cpu->memory[cPC]].length - 1;
	return instructions[cPC].cycles;
}

CPU* CPU_Create()
{
	CPU *cpu = malloc(sizeof(CPU));
	cpu->memory = malloc(sizeof(u8) * (64 * 1024));

	cpu->registers.AF = 0x01B0;
	cpu->registers.BC = 0x0013;
	cpu->registers.HL = 0x014D;
	cpu->registers.SP = 0xFFFE;
	cpu->registers.PC = 0x0100;

	return cpu;
}
void CPU_Destroy(CPU* cpu)
{
	free(cpu->memory);
	free(cpu);
}
