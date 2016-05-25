#include "cpu.h"

u8 ReadByte(CPU *cpu)
{
	return cpu->memory[cpu->registers.PC];
}

u16 ReadShort(CPU* cpu)
{
	return cpu->memory[cpu->registers.PC] << 8 | cpu->memory[cpu->registers.PC+1];
}

void WriteByte(CPU* cpu)
{

}

void WriteShort(CPU* cpu, unsigned short addr, unsigned short val)
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

	cpu->registers.BC = ReadShort(cpu);
	//$core->registerC = $core->memoryRead($core->programCounter);
	//$core->registerB = $core->memoryRead(($core->programCounter + 1) & 0xFFFF);
	//$core->programCounter = ($core->programCounter + 2) & 0xFFFF;
}

//LD (BC), A
void ins_0x02(CPU* cpu)
{
	cpu->memory[cpu->registers.BC] = cpu->registers.A;
}

//INC BC
void ins_0x03(CPU *cpu)
{
	cpu->registers.BC++;
}

//INC B
void ins_0x04(CPU* cpu)
{
	cpu->registers.B++;
}

//DEC B
void ins_0x05(CPU* cpu)
{
	cpu->registers.B--;
}

//LD B, d8
void ins_0x06(CPU *cpu)
{
	cpu->registers.B = ReadByte(cpu);
}

//RCLA, TODO
void ins_0x07(CPU *cpu)
{
	//rotate carry left A
	cpu->registers.carry = ((cpu->registers.A & 0x80) == 0x80);
	cpu->registers.A << 1;
	cpu->registers.A += cpu->registers.carry;
	//reset flags
	cpu->registers.zero = cpu->registers.negative = cpu->registers.halfCarry = 0;
}

//LD (a16), SP
void ins_0x08(CPU* cpu)
{
	cpu->memory[ReadShort(cpu)] = cpu->registers.SP;
	//WriteShort(cpu, cpu->);
}

//ADD HL, BC
void ins_0x09(CPU* cpu)
{
	cpu->registers.HL = cpu->registers.BC;
}

//LD A, BC
void ins_0x0A(CPU* cpu)
{
	cpu->registers.A = cpu->registers.BC;
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

void ins_0x0E(CPU* cpu)
{
	cpu->registers.C = ReadByte(cpu);
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

int CPU_Execute(CPU* cpu)
{
	const unsigned short cPC = cpu->registers.PC;
	instructions[cpu->registers.PC++].exc(cpu);
	cpu->registers.PC += instructions[cPC].length - 1;
	return instructions[cPC].cycles;
}

CPU* CPU_Create()
{
	CPU *cpu = malloc(sizeof(CPU));


	return cpu;
}
void CPU_Destroy(CPU* cpu)
{
	free(cpu);
}
