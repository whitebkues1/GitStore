#ifndef PHYISCAL_H
#define PHYSICAL_H
#include "main.h"

// typedefs
typedef enum {noResponse, getResponse} responseAction;
typedef enum {noLog, getLog} logAction;


void dumpXferBuf(void);
void sendCommand(responseAction wantResponse, logAction wantLog);

uint8_t spi_master_transfer(uint8_t *p_buf, uint8_t *resp, uint32_t size);
uint8_t spi_transfer(uint8_t *p_buf, uint8_t *resp, uint32_t size);
bool get_moredata_value(void);

#endif
