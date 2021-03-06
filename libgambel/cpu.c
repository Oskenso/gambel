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

void DecReg(CPU* cpu, u8* reg)
{
	(*reg)--;

	if ( ! cpu->registers.carry) {
		cpu->registers.F = 0;
	}

	cpu->registers.negative = 1;

	cpu->registers.zero = ( (*reg) == 0);
	cpu->registers.halfCarry = (((*reg) & 0x0F) == 0x0F);


	return 0;
}

void IncReg(CPU* cpu, u8* reg)
{
	//*reg++;//tODO : CHECK
	//reg++;
	/*
	*reg += 1;
	cpu->registers.negative = 0;

	if (cpu->registers.B)
		cpu->registers.zero = 1;
	else
		cpu->registers.zero = 0;

	if ((cpu->registers.B & 0xF) == 0xF)
		cpu->registers.halfCarry = 1;
	else
		cpu->registers.halfCarry = 0;*/

	if ((*reg & 0x0F) == 0x0F)
		cpu->registers.halfCarry = 1;
	else
		cpu->registers.halfCarry = 0;

	(*reg)++;
	if (*reg)
		cpu->registers.zero = 0;
	else
		cpu->registers.zero = 1;
	cpu->registers.negative = 0;
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
	cpu->registers.halfCarry = ((cpu->registers.B & 0xF) == 0xF);
	cpu->registers.zero = (cpu->registers.B == 0);

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
	IncReg(cpu, &cpu->registers.C);
	/*
	if ((cpu->registers.C & 0x0F) == 0x0F)
		cpu->registers.halfCarry = 1;
	else
		cpu->registers.halfCarry = 0;

	cpu->registers.C++;
	if (cpu->registers.C)
		cpu->registers.zero = 0;
	else
		cpu->registers.zero = 1;
	cpu->registers.negative = 0;
	*/
	return 0;
}

u8 ins_0x0D(CPU* cpu)
{
	DecReg(cpu, &cpu->registers.D);
	return 0;
}

// LD C, d8
u8 ins_0x0E(CPU* cpu)
{
	cpu->registers.C = ReadNext(cpu);
	return 0;
}

//LD DE,d16
u8 ins_0x11(CPU* cpu)
{
	cpu->registers.DE = ReadNextShort(cpu);
	return 0;
}

u8 ins_0x13(CPU* cpu)
{
	cpu->registers.DE--;
	return 0;
}


//RLA
u8 ins_0x17(CPU* cpu)
{

	u8 carry = (cpu->registers.A >= 0x80);
	cpu->registers.A <<= 1;
	cpu->registers.A |= cpu->registers.carry;
	cpu->registers.carry = carry;


	/*

    u8 carry = cpu->registers.carry;
    if (cpu->registers.A & 0x80)
        cpu->registers.carry = 1;
	else
		cpu->registers.carry = 0;

    cpu->registers.A <<= 1;
	cpu->registers.A |= carry;
*/
	cpu->registers.zero = (cpu->registers.A == 0);
	cpu->registers.negative = cpu->registers.halfCarry = 0;

    return 0;
}

u8 ins_0x18(CPU* cpu)
{
	cpu->registers.PC += cpu->memory[cpu->registers.PC];
	return 0;
}

//LD A,(DE)
u8 ins_0x1A(CPU* cpu)
{
    cpu->registers.A = cpu->memory[cpu->registers.DE];
	return 0;
}

//JR NZ,r8
u8 ins_0x20(CPU* cpu)
{
	//todo do stuff
	if (cpu->registers.zero == 1)
		return 0;

	cpu->registers.PC += (int8_t) ReadNext(cpu);

	//return  4;
	return 4;
}

u8 ins_0x21(CPU* cpu)
{
	cpu->registers.HL = ReadNextShort(cpu);
	return 0;
}

u8 ins_0x22(CPU* cpu)
{
	cpu->memory[cpu->registers.HL] = cpu->registers.A;
	cpu->registers.HL++;
	return 0;
}

u8 ins_0x23(CPU* cpu)
{
	cpu->registers.HL++;
	return 0;
}

u8 ins_0x27(CPU* cpu)
{
	u16 s = cpu->registers.A;


	if ( ! cpu->registers.negative) {
		if (cpu->registers.halfCarry || ((s & 0xf) > 9))
			s += 0x6;
		if (cpu->registers.carry || (s > 0x9f))
			s += 0x6;

	} else {
		if (cpu->registers.halfCarry)
			s = (s - 0) & 0xFF;
		if (cpu->registers.carry)
			s -= 0x60;

	}

	cpu->registers.halfCarry = 0;
	cpu->registers.zero = 0;

	if ((s & 0x100) == 0x100)
		cpu->registers.carry = 1;
	cpu->registers.zero =  ((s & 0xFF) == 0);

/*
	if(cpu->registers.negative) {
		if(cpu->registers.halfCarry) s = (s - 0x06)&0xFF;
		if(cpu->registers.carry) s -= 0x60;
	}
	else {
		if(cpu->registers.halfCarry || (s & 0xF) > 9) s += 0x06;
		if(cpu->registers.carry || s > 0x9F) s += 0x60;
	}

    cpu->registers.A = s & 0xFF;
	cpu->registers.halfCarry = 0;

	if(cpu->registers.A)
		cpu->registers.zero = 0;
	else
		cpu->registers.zero = 1;

	if(s >= 0x100)
		cpu->registers.carry = 1;
*/

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

//DEC A
u8 ins_0x3D(CPU* cpu)
{
	DecReg(cpu, &cpu->registers.A);
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

//LD C,A
u8 ins_0x4F(CPU* cpu)
{
	cpu->registers.C = cpu->registers.A;
	return 0;
}

//LD (HL),A
u8 ins_0x77(CPU* cpu)
{
	cpu->memory[cpu->registers.HL] = cpu->registers.A;
	return 0;
}

//LD A,E
u8 ins_0x7B(CPU* cpu)
{
	cpu->registers.A = cpu->registers.E;
	return 0;
}

//POP BC
u8 ins_0xC1(CPU* cpu)
{
	cpu->registers.BC = MemReadShort(cpu, cpu->registers.SP);
	cpu->registers.SP += 2;
	return 0;
}

u8 ins_0xC3(CPU* cpu)
{
	cpu->registers.PC = ReadNextShort(cpu);
	cpu->registers.PC -= 2;
	return 0;
}

//PUSH BC
u8 ins_0xC5(CPU* cpu)
{
	cpu->registers.SP -= 2;
	MemWriteShort(cpu, cpu->registers.SP, cpu->registers.BC);
	return 0;
}

u8 ins_0xC9(CPU* cpu)
{
	//RET 1  16 - - - -
	cpu->registers.PC = cpu->memory[cpu->registers.SP];
	cpu->registers.SP += 2;
	return 0;
}

u8 ins_0xCD(CPU* cpu)
{

	u16 npc = ReadNextShort(cpu);
	cpu->registers.SP -= 2;
	MemWriteShort(cpu, cpu->registers.SP, cpu->registers.PC+2 );
	cpu->registers.PC = npc - 2;

/*
	cpu->registers.SP -= 2;
	MemWriteShort(cpu, cpu->registers.SP, cpu->memory[cpu->registers.PC]);
	cpu->registers.PC = cpu->memory[cpu->registers.PC];
	cpu->registers.PC -= 4;
	*/

	return 0;
}


//AND E -- 1  4 -- Z 0 1 0
u8 ins_0xA3(CPU* cpu)
{

	cpu->registers.E = cpu->registers.A & cpu->registers.E;
	cpu->registers.halfCarry = 1;
	cpu->registers.zero = (cpu->registers.E == 0);
	cpu->registers.negative = 0;
	cpu->registers.carry = 0;
	/*

	cpu->registers.E &= cpu->memory[cpu->registers.PC];
	cpu->registers.E &= cpu->registers.E;
	if (cpu->registers.E == 0)
		cpu->registers.zero = 1;
	else
		cpu->registers.zero = 0;

	cpu->registers.negative = 0;
	cpu->registers.halfCarry = 1;
	cpu->registers.carry = 1;*/
	return 0;
}


// XOR A
u8 ins_0xAF(CPU* cpu)
{

	cpu->registers.carry = cpu->registers.halfCarry = cpu->registers.negative = 0;
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

//LD (C),A
u8 ins_0xE2(CPU* cpu)
{
	cpu->memory[0xff00 + cpu->registers.C] = cpu->registers.A;
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

u8 ins_crash(CPU* cpu)
{

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
	{"DEC D", 1, 4, ins_0x0D},

	{"LD C,d8", 2, 8, ins_0x0E},
	{"", 0, 0, ins_crash},


	{"", 0, 0, ins_crash},

	{"LD DE,d16", 3, 12, ins_0x11},
	{"", 0, 0, ins_crash},
	{"INC DE", 1, 8, ins_0x13},
{"", 0, 0, ins_crash},

{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
    {"RLA", 1, 4, ins_0x17},
	{"JR r8", 2, 12, ins_0x18},
	{"", 0, 0, ins_crash},

	{"LD A,(DE)", 1, 8, ins_0x1A},
	{"", 0, 0, ins_crash},
	{"", 0, 0, ins_crash},
	{"", 0, 0, ins_crash},
	{"", 0, 0, ins_crash},
	{"", 0, 0, ins_crash},



	{"JR NZ,r8", 2, 8, ins_0x20},
	{"LD HL,d16", 3, 12, ins_0x21},
	{"LD (HL+), A", 1, 8, ins_0x22},//LD (HL+),A 1  8 - - - -
	{"INC HL", 1, 8, ins_0x23},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"DAA", 1, 4, ins_0x27},
	{"JR Z,r8", 2, 8, ins_0x28},
	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},
//2

	//3
	{"", 0, 0, ins_crash},

	{"LD SP,d16", 3, 12, ins_0x31},
	{"LD (HL-),A", 1, 8, ins_0x32},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"DEC A", 1, 4, ins_0x3D},

	{"LD A,d8", 2, 8, ins_0x3E},
	{"", 0, 0, ins_crash},
//3

	//4
	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"LD B,A", 1, 4, ins_0x47},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"LD C,A", 1, 4, ins_0x4F},//4

	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//5
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//6

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},

	{"LD (HL),A", 1, 8, ins_0x77},
	{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"LD A, E", 1, 4, ins_0x7B},//LD A,E 1  4
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},


	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},//8
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"AND E", 1, 4, ins_0xA3},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//A
	{"XOR A", 1, 4, ins_0xAF},//A
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//B
	{"", 0, 0, ins_crash},
{"POP BC", 1, 12, ins_0xC1},
{"", 0, 0, ins_crash},

	{"JP a16", 3, 26, ins_0xC3},
	{"", 0, 0, ins_crash},

	{"PUSH BC", 1, 16, ins_0xC5},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"RET", 1, 16, ins_0xC9},
{"", 0, 0, ins_crash},

	{"PREFIX CB", 1, 4, ins_nop}, //nope ;3
	{"", 0, 0, ins_crash},

	{"CALL a16", 3, 24, ins_0xCD},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//C
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//D


	{"LDH (a8),A", 2, 12, ins_0xE0},
	{"", 0, 0, ins_crash},

	{"LD (C),A", 1, 8, ins_0xE2},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"LD (a16),A", 3, 16, ins_0xEA},{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//E
	{"LDH A,(a8)", 2, 12, ins_0xF0},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"DI", 1, 4, ins_0xF3},{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"CP d8", 2, 8, ins_0xFE},
	{"", 0, 0, ins_crash},
//F

};


//RL C
u8 ins_0xCB11(CPU* cpu)
{
	u8 carry = (cpu->registers.C >= 0x80);
	cpu->registers.C <<= 1;
	cpu->registers.C |= cpu->registers.carry;
	cpu->registers.carry = carry;
	cpu->registers.halfCarry = cpu->registers.negative = 0;

	cpu->registers.zero = (cpu->registers.C == 0);
	return 0;
}

u8 ins_0xCB7C(CPU* cpu)
{
	cpu->registers.halfCarry = 1;
	cpu->registers.negative = 0;

	cpu->registers.zero = ((cpu->registers.H & 128) == 0);


	return 0;
}

const INSTRUCTION instructionsCB[256] = {
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//0
	{"", 0, 0, ins_crash},

	{"RL C", 2, 8, ins_0xCB11},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//1
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//2
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//3
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//4
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//5
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//6


	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},

	{"BIT 7,H", 2, 8, ins_0xCB7C},
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//7


	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//8
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//9
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//A
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//B
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//C
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//D
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//E
	{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
{"", 0, 0, ins_crash},
//F

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
	cpu->registers.DE = 0;
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
