#include "tpm_demo.h"
#include "usart.h"



uint8_t menuHandler(void)
{
	uint8_t	option;
	printf("\033[2J" );	// clear VT-100 screen
	printf("\033[H"  );	// home cursor

	printf("\033[2J" );	// clear VT-100 screen
	printf("\033[H"  );	// home cursor

	printf("\r\n    SPI Demo for TPM - Version 2.0" );
	printf("\r\n (for AT97SSC3205P SPI protocol only)" );

	printf("\r\n" );
	printf("\r\n These are all available commands:" );
	printf("\r\n\r\n" );
	printf("\r\n1  TPM_Startup(ST_CLEAR)" );
	printf("\r\n2  TPM_ContinueSelfTest" );
	printf("\r\n3  TPM_CreateEKPair" );
	printf("\r\n4  TPM_TakeOwnerShip (sequence)" );
	printf("\r\n5  TPM_CreateWrapKey (sequence)" );
	printf("\r\n6  TPM_Loadkey (sequence)" );
	printf("\r\n7  TPM_Seal (sequence)" );
	printf("\r\na  TPM_UnSeal (sequence)" );
	printf("\r\nb  TPM_Sign (sequence)" );
	printf("\r\nc  TPM_VerifySign (sequence)" );
	printf("\r\nd  TPM_GetPubKey" );
	printf("\r\n" );
	printf("\r\nr  TPM_GeneratePrime128");
	printf("\r\nt  TPM_GetCapability (versionVal)" );
	printf("\r\nu  TPM_Reset (clears authSessions)" );
	printf("\r\nv  TPM_FlushSpecifc - keyHandle" );
	printf("\r\nw  TPM_ForceClear (sequence)" );
	printf("\r\nx  enable/activate TPM (sequence)" );
	printf("\r\ny  disable/deactivate TPM (sequence)" );
	printf("\r\nz  display known keys" );
	printf("\r\n" );
	printf("\r\n" );
//	printf( "\r\nq)  quit demo                   ");

	printf("\r\n\r\nplease pick a command: ");
    scanf( " %c", &option);
	return option;
}


void doCommand(uint8_t menuOption)
{
	switch(menuOption)
	{
	case '1':
		commandHandler("TPM_Startup_Clear");
		break;

	default:
		printf("\r\nInvalid option: %c", menuOption);
		scanf("%c", &menuOption);
	}

}

