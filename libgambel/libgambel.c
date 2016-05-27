#include "libgambel.h"


GB* GB_Create()
{
	GB *gb = (GB*) malloc(sizeof(GB));
	gb->cpu->registers.BC = 0xABCD;
	printf("%02x", gb->cpu->registers.B);
	printf("%02x", gb->cpu->registers.C);
	//CPU *cpu = CPU_Create();
	gb->cpu = CPU_Create();

	return gb;
}

void GB_LoadRom(GB* gb, u8 *romData)
{

}

void GB_Destroy(GB* gb)
{
	CPU_Destroy(gb->cpu);
	free(gb);
}
