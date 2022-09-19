#include "commandfuncs.h"
#include "usart.h"
#include "globals.h"
#include "tpm_errors.h"
#include "utils.h"
#include "physical.h"






























TPM_Command *findCommand(const char *commandName)
{
	TPM_Command *pCommand;
	uint16_t	i;

	for(i=0; i < numCommands; i++)
	{
		pCommand = &commandTable[i];
		if(! (strcmp(commandName, (char*) pCommand->commandName )) )
			return pCommand;
	}
	/* not found... */
	return (TPM_Command *) NULL;
}


TPM_FuncPtr *findFuncPtr(const char *commandName)
{
	TPM_FuncPtr *pFuncPtr;
	uint16_t	i;

	for(i=0; i < numCommands; i++)
	{
		pFuncPtr = &FuncPtrTable[i];
		if(! (strcmp(commandName, (char*) pFuncPtr->commandName )) )
		return pFuncPtr;
	}
	/* not found... */
	return (TPM_FuncPtr *) NULL;
}

/*****************************************************************************************************/
void init_xferbuf()
{
	uint16_t	i;
	for(i=0; i<sizeof(xferBuf); i++)
		xferBuf[i]=0;
}
/*****************************************************************************************************/


bool commandHandler(const char* commandName)
{
	TPM_Command *pCommand;
	TPM_FuncPtr *pFuncPtr;
	uint32_t	errCode =0;

	funcPtr		commandFunc;
 	bool 		returnVal =0;
	uint8_t		i;

	/* special cases, print user friendly name,... definitely redo code */
	if(strcmp(commandName,"TPM_GetCap_PROP_OWNER")== 0)
		printf("\r\n\r\nCommand called: TPM_GetCap(PROP_OWNER)" );
	else if (strcmp(commandName,"contSelfTest")== 0)
		printf("\r\n\r\nCommand called: TPM_ContinueSelfTest" );
	else if (strcmp(commandName,"createEKPair")== 0)
		printf("\r\n\r\nCommand called: TPM_CreateEndorsementKeyPair" );
	else if (strcmp(commandName,"readEK")== 0)
		printf("\r\n\r\nCommand called: TPM_ReadPubEK" );
	else if (strcmp(commandName,"OIAP")== 0)
		printf("\r\n\r\nCommand called: TPM_OIAP" );
	else if (strcmp(commandName,"generateRandN")==0)
		printf("\r\n\r\nCommand called:generateRandN");
	else if (strcmp(commandName,"takeOwnership")== 0)
	{
		printf("\r\n\r\nCommand called: TPM_TakeOwneship" );
		printf("\r\nTaking Ownership. Please wait... ");
	}
	else
 	 printf("\r\n\r\nCommand called: %s", commandName );

	if( (pCommand = findCommand(commandName)) == NULL )
	{
		printf("\r\ncommand not found: %s", commandName );
		return false;
	}

	if( (pFuncPtr = findFuncPtr(commandName)) == NULL )//�������
	{
		printf("\r\ncommand not found: %s", commandName );
		return false;
	}

	init_xferbuf();

	memcpy(&xferBuf[0],pCommand->commandBytes,pCommand->commandSize);

	/* set numBytes */
	numBytes = pCommand->commandSize;

	/* call command function */
	commandFunc = (funcPtr) pFuncPtr->commandFunc;
	commandFunc(pCommand);

	if(convertArrayToLong(((TPM_return*) xferBuf)->returnCode)  != TPM_SUCCESS )//TPM_SUCCESS��ֵΪ0
		{
			printf("\r\n%s failed!" , commandName);
			error_notification(pCommand);
			returnVal = true;
		}
		else
		{
			 /* POST Processing */
			 printf("\r\nTPM_SUCCESS");
			 //printf("\r\ndone!");

			success_post_processing(pCommand);
			returnVal = false;
		}

	printf("\r\npress any key to continue...");
	scanf( ("%c"), &i);

	return returnVal;
}


/*****************************************************************************************************/


void genericFunc( TPM_Command *pCommand )
{
	// generic send/receive routine (generally used for 0Auth commands)
	// can also be used for compliance mode (fixed) vectors
	// printf("\r\n    genericFunc called");

	// send/receive TPM bytes
	sendCommand(getResponse, getLog);

}






/* error notification */
/* error notification */
/* error notification */
void error_notification(TPM_Command *pCommand)
{
	uint32_t errCode = convertArrayToLong(((TPM_return*) xferBuf)->returnCode);


	/* error notification */
	switch(pCommand->commandTPM)
	{

		case TPM_CMD_STARTUP_CLEAR:
			if(errCode == 0x26)
				printf("\r\nTPM_POSTINIT error.\r\nTPM_Startup_Clear command can only be executed once per POR");
		break;

		case TPM_CMD_PHYSPRES_PRESENT:
			if(errCode == 0x03)
				printf("\r\nTPM_BAD_PARAMETER error.\r\nPhysical Presence command can not be executed.");
		break;

		case TPM_CMD_CREATE_EKPAIR:
			if(errCode == 0x08)
				printf("\r\nTPM_CMD_DISABLED error.\r\nEndorsement Key already Created.");
		break;

		case TPM_CMD_CONT_SELFTEST:
			if(errCode == 0x26)
				printf("\r\nTPM_POSTINIT error.\r\nTPM_Startup_Clear command can only be executed once per POR");
		break;
		case TPM_CMD_TAKE_OWNERSHIP:
		case TPM_CMD_OIAP:///Taken out of code
		case TPM_CMD_OSAP:///Taken out of code
		case TPM_CMD_OSAP_ET_KH_SRK:
		case TPM_CMD_CREATE_WRAPKEY:
		case TPM_CMD_LOADKEY:
		case TPM_CMD_READKEY:
		case TPM_CMD_SEAL:
		case TPM_CMD_UNSEAL:
		case TPM_CMD_SIGN:
		case TPM_CMD_VERIFY_SIGN:
		case TPM_CMD_GET_PUB_KEY:
		case TPM_CMD_BINDV20_G:
		case TPM_CMD_GET_CAP_VERSVAL:
		case TPM_CMD_RESET:
		case TPM_CMD_FLUSHKEY:
		case TPM_CMD_FORCE_CLEAR:
		case TPM_CMD_PHYS_SET_DEACT_FALSE:
		case TPM_CMD_PHYS_DISABLE:
		case TPM_CMD_PHYS_ENABLE:
		case TPM_GETCAP_VERSION:
		case TPM_CMD_PHY_SET_DEACT_TRUE:
		case TPM_CMD_GET_VERSION:
		case TPM_CMD_CREATE_WRAPKEY_PCR:
		case TPM_CMD_TERMINATE_HANDLE1:///Taken out of code
		case TPM_CMD_TERMINATE_HANDLE0:///Taken out of code
		case TPM_CMD_GET_SET_MEM:///Taken out of code
		case TPM_CMD_GET_CAP_PROP_OWNER:
		case TPM_CMD_OSAP_ET_KEY_SRK:


		if(errCode == 0x26)
			printf("\r\nTPM_POSTINIT error.\r\nTPM_Startup_Clear command must be first command executed.");

		break;

	}
}



void success_post_processing(TPM_Command *pCommand)
{
	/* command requiring post-processing */
	switch(pCommand->commandTPM)
	{

		case TPM_CMD_GET_CAP_VERSVAL:
			printf("\r\nTPM version: %02X.%02X.%02X.%02X", xferBuf[16], \
					xferBuf[17],xferBuf[18],xferBuf[19]);
		break;

		case TPM_CMD_STARTUP_CLEAR:
		case TPM_CMD_PHYSPRES_PRESENT:
		case TPM_CMD_CREATE_EKPAIR:
		case TPM_CMD_CONT_SELFTEST:
		case TPM_CMD_TAKE_OWNERSHIP:
		case TPM_CMD_OIAP:///Taken out of code
		case TPM_CMD_OSAP:///Taken out of code
		case TPM_CMD_OSAP_ET_KH_SRK:
		case TPM_CMD_CREATE_WRAPKEY:
		case TPM_CMD_LOADKEY:
		case TPM_CMD_READKEY:
		case TPM_CMD_SEAL:
		case TPM_CMD_UNSEAL:
		case TPM_CMD_SIGN:
		case TPM_CMD_VERIFY_SIGN:
		case TPM_CMD_GET_PUB_KEY:
		case TPM_CMD_BINDV20_G:
		case TPM_CMD_RESET:
		case TPM_CMD_FLUSHKEY:
		case TPM_CMD_FORCE_CLEAR:
		case TPM_CMD_PHYS_SET_DEACT_FALSE:
		case TPM_CMD_PHYS_DISABLE:
		case TPM_CMD_PHYS_ENABLE:
		case TPM_GETCAP_VERSION:
		case TPM_CMD_PHY_SET_DEACT_TRUE:
		case TPM_CMD_GET_VERSION:
		case TPM_CMD_CREATE_WRAPKEY_PCR:
		case TPM_CMD_TERMINATE_HANDLE1:///Taken out of code
		case TPM_CMD_TERMINATE_HANDLE0:///Taken out of code
		case TPM_CMD_GET_SET_MEM:///Taken out of code
		case TPM_CMD_GET_CAP_PROP_OWNER:
		case TPM_CMD_OSAP_ET_KEY_SRK:
		break;

	}

}



