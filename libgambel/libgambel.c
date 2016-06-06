#include "libgambel.h"


GB* GB_Create()
{
	GB *gb = (GB*) malloc(sizeof(GB));

	//CPU *cpu = CPU_Create();
	gb->cpu = CPU_Create();

	return gb;
}

void GB_LoadRom(GB* gb, uint8_t *romData)
{
	memcpy(&gb->cpu->memory[0x100], romData, 0x8000);
}

u8 GB_Step(GB* gb)
{
	return CPU_Execute(gb->cpu);

}

void GB_Destroy(GB* gb)
{
	CPU_Destroy(gb->cpu);
	free(gb);
}
