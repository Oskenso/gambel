#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "defines.h"

typedef struct {
	u8 zero:1;
	u8 negative: 1;
	u8 halfCarry: 1;
	u8 carry:1;

} FLAGS;

typedef struct {
	struct {
		union {
			struct {
				//unsigned char F;
				union {
					unsigned char F;
					struct {
						unsigned char unused: 4;
						unsigned char zero: 1;
						unsigned char negative: 1;
						unsigned char halfCarry: 1;
						unsigned char carry: 1;

					};
				};

				unsigned char A;
			};
			unsigned short AF;
		};
	};

	struct {
		union {
			struct {
				unsigned char C;
				unsigned char B;
			};
			unsigned short BC;
		};
	};

	struct {
		union {
			struct {
				unsigned char E;
				unsigned char D;
			};
			unsigned short DE;
		};
	};

	struct {
		union {
			struct {
				unsigned char L;
				unsigned char H;
			};
			unsigned short HL;
		};
	};

	unsigned short SP;
	unsigned short PC;
} REGISTERS;



typedef struct{
	FLAGS flags;
	REGISTERS registers;
	u8 memory[(1024 * 64)];

} CPU;

CPU* CPU_Create();
void CPU_Destroy(CPU*);

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
