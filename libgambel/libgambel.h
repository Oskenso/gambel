#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__MSC_VER)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT
#define IMPORT
#endif

#include "defines.h"
#include "cpu.h"
#include <string.h>

typedef struct {
	//u8 memory;
	CPU *cpu;
} GB;


EXPORT GB* GB_Create();
EXPORT void GB_Destroy(GB* gb);
EXPORT void GB_LoadRom(GB* gb, uint8_t *romData);
EXPORT u8 GB_Step(GB* gb);

#ifdef __cplusplus
}
#endif
