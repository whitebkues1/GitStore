#ifndef CMDFUNCS_H
#define CMDFUNCS_H
#include "main.h"
#include "commandstructs.h"


bool commandHandler (const char *str);

void genericFunc(  TPM_Command *pCommand );
void error_notification(TPM_Command *pCommand );
void success_post_processing(TPM_Command *pCommand);


#endif
