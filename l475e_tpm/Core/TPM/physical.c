#include "physical.h"
#include "usart.h"
#include "globals.h"
#include "spi.h"

#define SPI_READ_CMD	0x80
#define SPI_WRITE_CMD	0x00
#define CMD_SIZE		0x04
#define WAIT_STATE_MASK	0x0001
#define WAIT_STATE_REQUIRED 0x00

#define MAX_RETRIES		100

//-----------------------------------------------------------------------------
void dumpXferBuf(void)
{
	uint16_t	i;

	for(i=0; i < numBytes; i++)
	{
		if(!(i % 16))
		{
			printf("\r\n");
		}
		printf("%02X ", xferBuf[i] );
	}
	printf("\r\n");
}
//-----------------------------------------------------------------------------

uint8_t spi_master_transfer(uint8_t *p_buf, uint8_t *resp, uint32_t size)
{
	bool test_bit = false;
	if(*p_buf == SPI_WRITE_CMD)
	{
		if(size > 1)
		{
			/* (send size-1) bytes */
			spi_transfer(p_buf, resp, (size-1));
		}
	}

}

/**
 * \brief Perform SPI master transfer.
 *
 * \param pbuf Pointer to buffer to transfer.
 * \param size Size of the buffer.
 */
 uint8_t spi_transfer(uint8_t *p_buf, uint8_t *resp, uint32_t size)
 {
		uint16_t i;
		uint8_t uc_pcs;
		uint16_t data;
		bool updateCnt = false;
		uint8_t retry;

		union {
			uint8_t		bytes[2];
			uint16_t	size;
		} paramSize;

		/* send command bytes */
		for (i = 0; i < CMD_SIZE; i++) {
			HAL_SPI_Transmit(&hspi1, &p_buf[i], sizeof(p_buf), 1000);

			/* Read Data. */
			uint8_t tmpdata[2];
			HAL_SPI_Receive(&hspi1, tmpdata, sizeof(tmpdata), 1000);
			data = (uint16_t)tmpdata[0];
			data = (data << 8);
			data |= (uint16_t)tmpdata[1];
		}

		retry = 0;
		/* handle wait state before proceeding */
		do
		{
			/* check for wait state, loop 'til completed */
			if((data & WAIT_STATE_MASK) == WAIT_STATE_REQUIRED)
			{
				uint8_t tmp = 0xFF;
				HAL_SPI_Transmit(&hspi1, &tmp, sizeof(tmp), 1000);
				uint8_t tmpdata[2];
				HAL_SPI_Receive(&hspi1, tmpdata, sizeof(tmpdata), 1000);
				data = (uint16_t)tmpdata[0];
				data = (data << 8);
				data |= (uint16_t)tmpdata[1];
			}

			if(retry > MAX_RETRIES)
			{
				printf("\n\r Comm Error: WAIT STATE bit was never \
				\r\n was never  released by TPM device\r\n");
				return 1;
			}
			retry++;
		}while((data & WAIT_STATE_MASK) == WAIT_STATE_REQUIRED);

 }

void sendCommand(responseAction wantResponse, logAction wantLog)
{
	uint8_t cmd_buffer[16];
	uint8_t resp_buffer[64];
	uint8_t retry;
	uint8_t retry_tpmGo;
	/* this must remain static */
	static bool firstTime = true;

	/* show what we're sending */
	if(wantLog == getLog)
	{
		printf("\r\nto TPM:");
		dumpXferBuf();
	}

	/* notify user about takeownFunc's key generation */
	if(xferBuf[9] == 0x0D)
	{
		printf("\r\nPlease wait. Generating SRK ...");
	}

	/* notify user about createWrapFunc's key generation */
	if(xferBuf[9] == 0x1f)
	{
		printf("\r\nPlease wait. Generating key ...");
	}

	/* check locality register access */
	cmd_buffer[0] = SPI_READ_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x00;
	spi_master_transfer(&cmd_buffer[0],&resp_buffer[0], 0x01);


}
