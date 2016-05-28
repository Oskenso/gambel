#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "defines.h"

typedef struct {
	struct {
		union {
			struct {
				union {
					u8 F;
					struct {
						u8 unused: 4;
						u8 zero: 1;
						u8 negative: 1;
						u8 halfCarry: 1;
						u8 carry: 1;

					};
				};
				u8 A;
			};
			u16 AF;
		};
	};

	struct {
		union {
			struct {
				u8 C;
				u8 B;
			};
			u16 BC;
		};
	};

	struct {
		union {
			struct {
				u8 E;
				u8 D;
			};
			u16 DE;
		};
	};

	struct {
		union {
			struct {
				u8 L;
				u8 H;
			};
			u16 HL;
		};
	};

	u16 SP;
	u16 PC;
} REGISTERS;



typedef struct{
	REGISTERS registers;
	//u8 memory[(1024 * 64)];
	u8 *memory;

} CPU;

CPU* CPU_Create();
void CPU_Destroy(CPU*);
u8 CPU_Execute(CPU* cpu);

typedef struct {

	char *name;
	u8 length;
	u8 cycles;
	//FLAGS flags; // flags affected
	void (*exc)(CPU*);


} INSTRUCTION;





#ifdef __cplusplus
}
#endif
