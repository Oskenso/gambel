#ifndef LIBGAMBEL_H
#define LIBGAMBEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "defines.h"

struct GB {
	u8 memory;
};
typedef struct GB GB;

GB* GB_Create();
void GB_Destroy(GB* gb);

#ifdef __cplusplus
}
#endif

#endif // LIBGAMBEL_H
