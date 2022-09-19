#include "commandstructs.h"
#include "commandfuncs.h"


static uint8_t _TPM_Startup_Clear[] = "TPM_Startup_Clear";
static uint8_t __TPM_Startup_Clear[]  =
{
	0x00, 0xC1,
	0x00, 0x00, 0x00, 0x0C,
	0x00, 0x00, 0x00, 0x99,
	0x00, 0x01,
};


#define tableEnt(entryName, tpmCmd, numAuth, inHandles, outHandles) \
{_##entryName, __##entryName, sizeof(__##entryName), tpmCmd, numAuth, inHandles, outHandles },


/*
 *	The global table of commands (and meta-commands)
 */
TPM_Command commandTable[] =
{

	//commandName, 					tableIndex					numAuths,	numInHandles,	numOutHandles
	tableEnt(TPM_Startup_Clear,		TPM_CMD_STARTUP_CLEAR,		AUTH0,		HAND0,			HAND0)

};

 #define tableEntFuncPtr(entryName, funcName){_##entryName, funcName},


TPM_FuncPtr FuncPtrTable[] =
{

	//commandName, 				commandFunc
	tableEntFuncPtr(TPM_Startup_Clear,		genericFunc)

};


/* global var exposing number of commands */
uint16_t numCommands = NUMCOMMANDS;

/* global var exposing size of command table */
uint16_t commandTableSize = CMDTABLESIZE;
