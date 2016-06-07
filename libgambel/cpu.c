#include "cpu.h"
#include <stdio.h>
u8 ReadNext(CPU *cpu)
{
	return cpu->memory[cpu->registers.PC];
}

u16 ReadNextShort(CPU* cpu)
{
	return cpu->memory[cpu->registers.PC] | (cpu->memory[cpu->registers.PC+1] << 8);
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

void IncReg(CPU* cpu, u8* reg)
{
	//*reg++;//tODO : CHECK
	//reg++;
	*reg += 1;
	cpu->registers.negative = 0;

	if (cpu->registers.B)
		cpu->registers.zero = 1;
	else
		cpu->registers.zero = 0;

	if ((cpu->registers.B & 0xF) == 0xF)
		cpu->registers.halfCarry = 1;
	else
		cpu->registers.halfCarry = 0;
}


//Do a thing :D
u8 ins_nop(CPU *cpu)
{
	//do nothing but consume 4 cycles
	return 0;
}
//LD BC, d16
u8 ins_0x01(CPU* cpu)
{
	cpu->registers.BC = ReadNextShort(cpu);
	return 0;
}

//LD (BC), A
u8 ins_0x02(CPU* cpu)
{
	cpu->memory[cpu->registers.BC] = cpu->registers.A;
	return 0;
}

//INC BC - no flags
u8 ins_0x03(CPU *cpu)
{
	cpu->registers.BC++;
	return 0;
}

//INC B - Z 0 H -
u8 ins_0x04(CPU* cpu)
{
	IncReg(cpu, &cpu->registers.B);
	return 0;
	/*
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
	*/
}

//DEC B - Z 1 H -
u8 ins_0x05(CPU* cpu)
{
	cpu->registers.B--;
	cpu->registers.zero = (cpu->registers.B > 0);
	cpu->registers.halfCarry = ((cpu->registers.B & 0xF) == 0xF);
	cpu->registers.negative = 1;
	return 0;
}

//LD B, d8
u8 ins_0x06(CPU *cpu)
{
	cpu->registers.B = ReadNext(cpu);
	return 0;
}

//RCLA - 0 0 0 C
u8 ins_0x07(CPU *cpu)
{
	cpu->registers.carry = cpu->registers.A & 1;

	cpu->registers.A <<= 1;
	cpu->registers.A += cpu->registers.carry;

	cpu->registers.zero = cpu->registers.negative = cpu->registers.halfCarry = 0;
	return 0;
}

//LD (a16), SP
u8 ins_0x08(CPU* cpu)
{
	MemWriteShort(cpu, ReadNextShort(cpu), cpu->registers.SP);
	return 0;
}

//ADD HL, BC  -  - 0 H C
u8 ins_0x09(CPU* cpu)
{
	cpu->registers.carry = (cpu->registers.HL + cpu->registers.BC) > 0xFFFF;

	cpu->registers.HL = cpu->registers.BC;

	cpu->registers.halfCarry = (((cpu->registers.HL & 0xF) + (cpu->registers.BC & 0xF)) > 0xF);

	cpu->registers.negative = 0;
	return 0;
}

//LD A, (BC)
u8 ins_0x0A(CPU* cpu)
{
	cpu->registers.A = cpu->memory[cpu->registers.BC];
	return 0;
}

//
u8 ins_0x0B(CPU* cpu)
{
	cpu->registers.BC--;
	return 0;
}

u8 ins_0x0C(CPU* cpu)
{
	cpu->registers.C++;
	return 0;
}

u8 ins_0x0D(CPU* cpu)
{
	cpu->registers.D--;
	return 0;
}

// LD C, d8
u8 ins_0x0E(CPU* cpu)
{
	cpu->registers.C = ReadNext(cpu);
	return 0;
}

u8 ins_0x18(CPU* cpu)
{
	cpu->registers.PC += cpu->memory[cpu->registers.PC];
	//cpu->registers.PC -= 2;
	return 0;
}

//JR NZ,r8
u8 ins_0x20(CPU* cpu)
{
	//todo do stuff
	if (cpu->registers.zero == 0)
		return 0;

	cpu->registers.PC += ReadNext(cpu);

	//return  4;
	return 4;
}

u8 ins_0x21(CPU* cpu)
{
	cpu->registers.HL = ReadNextShort(cpu);
	return 0;
}

u8 ins_0x28(CPU* cpu)
{
	if ( ! cpu->registers.F) {
		cpu->registers.PC += cpu->memory[cpu->registers.PC];
		return 4;
	}
	return 0;
}

//LD SP,d16
u8 ins_0x31(CPU* cpu)
{
	cpu->registers.SP = ReadNextShort(cpu);
	return 0;
}

//"LD (HL-),A
u8 ins_0x32(CPU* cpu)
{
	cpu->memory[cpu->registers.HL] = cpu->registers.A;
	cpu->registers.HL--;
	return 0;
}

u8 ins_0x3E(CPU* cpu)
{
	cpu->registers.A = cpu->memory[cpu->registers.PC];
	return 0;
}

//LD B,A 1, 4
u8 ins_0x47(CPU* cpu)
{
	cpu->registers.B = cpu->registers.A;
	return 0;
}

u8 ins_0xC3(CPU* cpu)
{
	cpu->registers.PC = ReadNextShort(cpu);
	cpu->registers.PC -= 2;
	return 0;
}

u8 ins_0xCD(CPU* cpu)
{
	//cpu->registers.SP;
	cpu->registers.SP = cpu->registers.PC - 2;
	cpu->registers.PC = cpu->memory[cpu->registers.PC];
	cpu->registers.PC -= 2;
	return 0;
}



// XOR A
u8 ins_0xAF(CPU* cpu)
{

	cpu->registers.carry = cpu->registers.halfCarry = cpu->registers.negative = cpu->registers.zero = 0;
	//cpu->registers.A ^= cpu->registers.A;
	cpu->registers.A = 0;
		cpu->registers.zero = 1;
	return 0;
}

u8 ins_0xE0(CPU* cpu)
{
	MemWriteShort(cpu, cpu->memory[cpu->registers.PC] + 0xFF00, cpu->registers.A);
	return 0;
}

//LD (a16),A
u8 ins_0xEA(CPU* cpu)
{
	MemWriteShort(cpu, cpu->memory[cpu->registers.PC], cpu->registers.A);
	return 0;
}





//LD A,($FF00+a8)
u8 ins_0xF0(CPU* cpu)
{
	cpu->registers.A = cpu->memory[0xFF00 + ReadNext(cpu)];
	return 0;
}

u8 ins_0xF3(CPU* cpu)
{

	//TODO INTERRUPT STUFFS
	return 0;
}

u8 ins_0xFE(CPU* cpu)
{
	cpu->registers.zero = (cpu->registers.A == cpu->memory[cpu->registers.PC]);
	cpu->registers.carry = (cpu->memory[cpu->registers.PC] > cpu->registers.A);
	cpu->registers.halfCarry = ((cpu->memory[cpu->registers.PC] & 0x0F) > (cpu->registers.A & 0x0F));
	cpu->registers.negative = 1;
	return 0;
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
	{"INC C", 1, 4, ins_0x0C},
	{},{},{},
	{},{},{},{},{},{},{},{},
	{"JR r8", 2, 12, ins_0x18},
	{},{},{},{},{},{},{},


	{"JR NZ,r8", 2, 8, ins_0x20},
	{"LD HL,d16", 3, 12, ins_0x21},
	{},{},{},{},{},{},
	{"JR Z,r8", 2, 8, ins_0x28},{},{},{},{},{},{},{},//2

	//3
	{},
	{"LD SP,d16", 3, 12, ins_0x31},
	{"LD (HL-),A", 1, 8, ins_0x32},
	{},{},{},{},{},{},{},{},{},{},{},
	{"LD A,d8", 2, 8, ins_0x3E},
	{},//3

	//4
	{},
	{},{},{},{},{},{},
	{"LD B,A", 1, 4, ins_0x47},
	{},{},{},{},{},{},{},{},//4
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//5
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//6
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//A
	{"XOR A", 1, 4, ins_0xAF},//A
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//B
	{},{},{},
	{"JP a16", 3, 26, ins_0xC3},{},{},{},{},{},{},{},
	{"PREFIX CB", 1, 4, ins_nop}, //nope ;3
	{},
	{"CALL a16", 3, 24, ins_0xCD},
	{},{},//C
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//D
	{"LDH (a8),A", 2, 12, ins_0xE0},{},{},{},{},{},{},{},{},{},
	{"LD (a16),A", 3, 16, ins_0xEA},{},{},{},{},{},//E
	{"LDH A,(a8)", 2, 12, ins_0xF0},
	{},{},
	{"DI", 1, 4, ins_0xF3},{},{},{},{},{},{},{},{},{},{},
	{"CP d8", 2, 8, ins_0xFE},
	{},//F

};


u8 ins_0xCB7C(CPU* cpu)
{
	cpu->registers.halfCarry = 1;
	cpu->registers.negative = 0;

	if (cpu->registers.H & (1 << 7))
		cpu->registers.zero = 1;
	else
		cpu->registers.zero = 0;
	cpu->registers.H = cpu->registers.H & (1 << 7);


	return 0;
}

const INSTRUCTION instructionsCB[256] = {
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//0
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//1
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//2
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//3
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//4
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//5
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//6


	{},{},{},{},{},{},{},{},{},{},{},{},
	{"BIT 7,H", 2, 8, ins_0xCB7C},
	{},{},{},//7


	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//8
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//9
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//A
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//B
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//C
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//D
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//E
	{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},//F

};

#include <stdio.h>
u8 CPU_Execute(CPU* cpu)
{
	const u16 cPC = cpu->registers.PC;
	//printf("%d", instructions[cpu->memory[cPC]].length);
	//printf("0x%04x", ReadNextShort(cpu));
	u8 opcode = cpu->memory[cpu->registers.PC];
	u8 cycles = 0;

	if (opcode == 0xCB) {
		cpu->registers.PC++;
		cycles = instructionsCB[cpu->memory[cpu->registers.PC++]].exc(cpu);

		//cpu->registers.PC++;
		return instructionsCB[cpu->memory[cPC]].cycles + cycles;
	} else {
		cycles = instructions[cpu->memory[cpu->registers.PC++]].exc(cpu);
		cpu->registers.PC += instructions[cpu->memory[cPC]].length - 1;
		return instructions[cpu->memory[cPC]].cycles + cycles;
	}


}

CPU* CPU_Create()
{
	CPU *cpu = malloc(sizeof(CPU));
	cpu->memory = malloc(sizeof(u8) * (64 * 1024));

	/*
	cpu->registers.AF = 0x01B0;
	cpu->registers.BC = 0x0013;
	cpu->registers.HL = 0x014D;
	cpu->registers.SP = 0xFFFE;
	cpu->registers.PC = 0x0100;
	*/
	cpu->registers.AF = 0;
	cpu->registers.BC = 0;
	cpu->registers.HL = 0;
	cpu->registers.SP = 0;
	cpu->registers.PC = 0;

	return cpu;
}
void CPU_Destroy(CPU* cpu)
{
	free(cpu->memory);
	free(cpu);
}
