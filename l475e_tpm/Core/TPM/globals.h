#ifndef GLOBALS_H
#define GLOBALS_H
#include "main.h"



// generic command output struct
typedef struct tdTPM_return
{
	uint8_t	tag[2];
	uint8_t	paramSize[4];
	uint8_t	returnCode[4];
	uint8_t	payLoad;				// marker
} TPM_return;


// IO buffer
extern uint8_t xferBuf[1024];

// number of bytes to send / receive
extern uint16_t numBytes;

#endif
