#include "libgambel.h"


GB* GB_Create()
{
	GB *gb = (GB*) malloc(sizeof(GB));
	//CPU *cpu = CPU_Create();
	gb->cpu = CPU_Create();

	return gb;
}

void GB_Destroy(GB* gb)
{
	CPU_Destroy(gb->cpu);
	free(gb);
}
