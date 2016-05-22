#include "libgambel.h"

GB* GB_Create()
{
	GB *gb = (GB*) malloc(sizeof(GB));
	return gb;
}

void GB_Destroy(GB* gb)
{
	free(gb);
}
