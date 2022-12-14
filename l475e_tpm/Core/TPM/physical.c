
#include "physical.h"

#include "commandStructs.h"
#include "authHandler.h"
#include "globals.h"


#include "utils.h"
#include "usart.h"
#include "spi.h"
//#include "delay.h"
#include "conf_board.h"

#define TPM_HEADER_SIZE		0x0A


// number of polling attempts to get first ACK
#define POLL_NUM		100
#define SPI_READ_CMD	0x80
#define SPI_WRITE_CMD	0x00
#define WAIT_STATE_MASK	0x0001
#define WAIT_STATE_REQUIRED 0x00
#define DUMMY_WAIT_BYTE	0xFF
#define CMD_SIZE		0x04
#define CMDRDY_MASK		0xC8
#define DATARDY_MASK	0x90
#define CMD_OFFSET			0x04
#define TRANSFER_SUCCESS    0x00

// global var to hold transient TWSR value
volatile uint8_t	twst;

#define DUMMY_BYTES		0xFF
#define RESP_SIZE_LSB	5
#define RESP_SIZE_MSB	4
#define MAX_RETRIES		100
#ifdef TWI_INTERFACE

uint8_t tpm_iic_test_data[] = {
	0x00, 0xC1, 0x00, 0x00,
	0x00, 0x0C, 0x00, 0x00,
	0x00, 0x99, 0x00, 0x01
};

//static const 
// Response
// Tag:  0x00 0xC4
// Size: 0x00 0x00 0x00 0x0a
// Return code: 00 00 00 00 (success)

 uint8_t tpm_iic_expected_test_data_resp[] = {
	0x00, 0xC4,
	0x00, 0x00, 0x00, 0x0A,
	0x00, 0x00, 0x00, 0x00
};


/** Data to be sent */
#define  IIC_TEST_DATA_LENGTH  (sizeof(tpm_iic_test_data)/sizeof(uint8_t))

#else

/* Chip select. */
#define SPI_CHIP_SEL 0

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 1

/* Delay before SPCK. */
#define SPI_DLYBS 0x40 

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x40

/* Number of SPI clock configurations. */
#define NUM_SPCK_CONFIGURATIONS 3

/* Data block number. */
#define MAX_DATA_BLOCK_NUMBER  4

/* Slave data state, begin to return last data block. */
#define SPI_CMD_READ     0x80
#define SPI_CMD_WRITE    0x00

/* SPI clock setting (Hz). */
static uint32_t gs_ul_spi_clock;

/* SPI Status. */
//static struct status_block_t gs_spi_status;


/* SPI clock configuration. */
//const uint32_t gs_ul_clock_configurations[] = {1000000, 10000000, 2000000 };

/**
 * \brief Initialize SPI as master.
 */



static void spi_master_initialize(void)
{
	/* Configure an SPI peripheral. */
//	spi_enable_clock(SPI_MASTER_BASE);
//	spi_disable(SPI_MASTER_BASE);
//	spi_reset(SPI_MASTER_BASE);
//	spi_set_lastxfer(SPI_MASTER_BASE);
//	spi_set_master_mode(SPI_MASTER_BASE);
//	spi_disable_mode_fault_detect(SPI_MASTER_BASE);
//
//	spi_set_peripheral_chip_select_value(SPI_MASTER_BASE, SPI_CHIP_SEL);
//	spi_set_clock_polarity(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
//	spi_set_clock_phase(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);
//	spi_set_bits_per_transfer(SPI_MASTER_BASE, SPI_CHIP_SEL,SPI_CSR_BITS_8_BIT);
//	spi_set_baudrate_div(SPI_MASTER_BASE, SPI_CHIP_SEL,(sysclk_get_cpu_hz() / gs_ul_spi_clock));
//	spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,SPI_DLYBCT);
//	spi_enable(SPI_MASTER_BASE);
}


/**
 * \brief Set the specified SPI clock configuration.
 *
 * \param configuration  Index of the configuration to set.
 */
void spi_set_clock_configuration(uint8_t configuration)
{
//	gs_ul_spi_clock = 1000000;//gs_ul_clock_configurations[configuration];
//	//printf("Setting SPI clock #%lu ... \n\r", (unsigned long)gs_ul_spi_clock);
//	spi_master_initialize();
}

#endif


void twi_init(void)
{
//	twi_options_t opt = {
//		.master_clk = sysclk_get_cpu_hz(),
//		.speed = TWI_CLK,
//		.chip  = TPM_DEVICE_ADDRESS
//	};
//
//	if (twi_master_setup(TPM_TWI_MODULE, &opt) != TWI_SUCCESS) {
//		/* to-do, error handling */
//		while(1);
//	}

}

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
void logStartTries(uint8_t numTries)
{
	printf("%d tries\r\n", numTries);
}
//-----------------------------------------------------------------------------
void convertHexToAsciiString( uint8_t hexData, char *ptrHexString )
{
	ptrHexString[0] = (hexData&0xF0)>>4;
	ptrHexString[0] += (ptrHexString[0] < 0x0A) ? 0x30 : 0x37;

	ptrHexString[1] = (hexData&0x0F);
	ptrHexString[1] += (ptrHexString[1] < 0x0A) ? 0x30 : 0x37;
}


#ifdef SPI_INTERFACE

uint8_t spi_master_transfer(uint8_t *p_buf, uint8_t *resp, uint32_t size)
{
//spi_transfer(p_buf, resp, size); 	
	bool test_bit = false;
	
	   if(*p_buf == SPI_WRITE_CMD)
	   {
	   
			if(size > 1)
			{
				
				/* (send size-1) bytes */
				spi_transfer(p_buf, resp, (size-1));
				test_bit = get_moredata_value();
			
				/* check and return status */
				if(test_bit)
				{

					xferBuf[0] = SPI_WRITE_CMD;
					xferBuf[1] = 0xD4;
					xferBuf[2] = 0x00;
					xferBuf[3] = 0x24;
					xferBuf[4] = *(p_buf + CMD_OFFSET +(size-1));
					spi_master_transfer(&xferBuf[0],resp,  0x01);

					/* re-check status */
					test_bit =  get_moredata_value();
					if(test_bit)
					{
					    /* if high, an error has occurred */
						return 1; // error
					}

				}
				else
					return 1; //error				
			}
			else if (size == 1)
			{
				spi_transfer(p_buf, resp, size); 				
			}

	   }
	   else /* Normal Read Operation */
			spi_transfer(p_buf, resp, size);
			
	return 0;			
}


bool get_moredata_value()
{

	uint8_t cmd_buffer[8];
	uint8_t resp_buffer[2];

	/* read status register */
	cmd_buffer[0] = SPI_READ_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x18;
	spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);
	
	/* return false if TPM is NOT expecting more data */
	return (bool)(resp_buffer[0] & 0x08);
}

bool flush_tpm()
{
	uint8_t cmd_buffer[8];
	uint8_t resp_buffer[2];
	uint8_t i;
	
	do 
	{
		cmd_buffer[0] = SPI_WRITE_CMD;
		cmd_buffer[1] = 0xD4;
		cmd_buffer[2] = 0x00;
		cmd_buffer[3] = 0x24;
		cmd_buffer[4] = 0xFF;
		
		spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);
		if(!get_moredata_value())
		{
			return false;	 
		}
			
		i++;
	} while (i < 200);
	
	return true;
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

	//cpu_irq_enter_critical();


	/* send command bytes */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	for (i = 0; i < CMD_SIZE; i++)
	{
		uint8_t temp = 0;
		HAL_SPI_TransmitReceive(&hspi1, &p_buf[i], &temp, 1, 1000);
		data = temp;

	}
	
	retry = 0;
	/* handle wait state before proceeding */
	do
	{
		/* check for wait state, loop 'til completed */
		if((data & WAIT_STATE_MASK) == WAIT_STATE_REQUIRED)
		{
			
			uint8_t tmp = 0xFF;
			uint8_t temp = 0;
			HAL_SPI_TransmitReceive(&hspi1, &tmp, &temp, 1, 1000);
			data = temp;


		}
		

		if(retry > MAX_RETRIES)
		{
			//cpu_irq_leave_critical();
			printf("\n\r Comm Error: WAIT STATE bit was never \
			\r\n was never  released by TPM device\r\n");
			return 1;
		}
		retry++;
		
		
			
	} while ((data & WAIT_STATE_MASK) == WAIT_STATE_REQUIRED);

	
	if((size == TPM_HEADER_SIZE)&&((*p_buf) == SPI_READ_CMD))
	{
		updateCnt = true;
	}
	
	
	/* process remaining data */
	if(size > 0)
	{
		/* send command bytes */
		for (i = 0; i < size; i++) 
		{
			if((*p_buf) == SPI_READ_CMD)
			{

				uint8_t tmp = 0xFF;
				uint8_t temp = 0;
				HAL_SPI_TransmitReceive(&hspi1, &tmp, &temp, 1, 1000);
				data = temp;
			}

			else
			{
				uint8_t temp = 0;
				HAL_SPI_TransmitReceive(&hspi1, &p_buf[CMD_SIZE + i], &temp, 1, 1000);
				data = temp;
			}

		
			*(resp+i) = data;
			
			
			/* extract the paramSize
			* big/little endian conversion,
			* this is faster than shifting)
			*/
			
			if((i == RESP_SIZE_LSB)&&(updateCnt == true)){
				paramSize.bytes[0] = *(resp + RESP_SIZE_LSB);
				paramSize.bytes[1] = *(resp + RESP_SIZE_MSB);
				numBytes = paramSize.size;
				updateCnt = false;

				if(numBytes > 1024){
					//cpu_irq_leave_critical();
					printf("\r\nretrieving response size\r\n");
					return 1;
				}
				 
				 
				/* update cnt */
				if(paramSize.size != TPM_HEADER_SIZE){
					size = paramSize.size;
				}
			}			

		}		
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	//cpu_irq_leave_critical();

	HAL_Delay(10);
	return 0;

}
#endif

//-----------------------------------------------------------------------------
void sendCommand(	responseAction 	wantResponse,
					logAction 		wantLog)
{

#ifdef TWI_INTERFACE
	uint16_t 	i;
	uint32_t	retCode;
	twi_packet_t packet_tx, packet_rx;
#else	
	uint8_t cmd_buffer[16];
	uint8_t resp_buffer[64];
	uint8_t retry;
	uint8_t retry_tpmGo;
	/* this must remain static */
	static bool firstTime = true;
	
#endif		

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
	


#ifdef TWI_INTERFACE

   /* get the TPM's attention */
	i = 0;

	/* Configure the data packet to be transmitted */
	packet_tx.chip        = TPM_DEVICE_ADDRESS;
	packet_tx.addr[0]     = 0x00;
	packet_tx.addr[1]     = 0x00;
	packet_tx.addr_length = 0x00;
	packet_tx.buffer      = xferBuf;
	packet_tx.length      = numBytes;	

	do{
		retCode =0;
		/* IIC command */
		retCode = twi_master_write(TPM_TWI_MODULE, &packet_tx);							
		i++;
		
		asm("nop");
		/* give POLL_NUM tries... */	 
	} while((retCode != TWI_SUCCESS) && (i < POLL_NUM));

#ifndef NO_STARTBIT_LOG
	if(i > 1)
	{
		printf("\r\nfirst write startBit took ");
		logStartTries(i);
		if(i == POLL_NUM)
		{
			printf("aborting (xmit ACK 1)\r\n");
			return;
		}
	}
#endif


	if(wantResponse == getResponse)
	{

		/* clear the xfer buffer */
		for(i=0; i<numBytes; i++)
			xferBuf[i]=0;

		/* wait for TPM to finish
		 * command execution production 
		 * code would want to have 
		 * (multiple-level) timeouts here
		 */
		do{
			retCode = twi_probe(TPM_TWI_MODULE, TPM_DEVICE_ADDRESS);	
		} while(retCode != PASS);


		/* Configure the data packet to be received */
		packet_rx.chip        = packet_tx.chip;
		packet_rx.addr[0]     = 0x00;
		packet_rx.addr[1]     = 0x00;
		packet_rx.addr_length = 0x00;
		packet_rx.buffer      = xferBuf;
		packet_rx.length      = TPM_HEADER_SIZE;

		twi_master_read_tpm(TPM_TWI_MODULE, &packet_rx);


		if( (numBytes > 1023) || (numBytes < 10) )
		{
			/* something's wrong!! */
			printf("bad paramSize(%d)\r\n",numBytes);
			return;	
		}

		/* show what we got */
		if(wantLog == getLog)
		{
			printf("\r\nfrom TPM:");
			dumpXferBuf();
		}
	}

#else // SPI

// if(firstTime)
// {
	/* check locality register access */	
	cmd_buffer[0] = SPI_READ_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x00;
	spi_master_transfer(&cmd_buffer[0],&resp_buffer[0], 0x01);
	
	retry = 0;
	
	/* expected results received */
	if(resp_buffer[0] != 0xA1)
	{
		do 
		{
 			/* send request use byte */
			cmd_buffer[0] = SPI_WRITE_CMD;
			cmd_buffer[1] = 0xD4;
			cmd_buffer[2] = 0x00;
			cmd_buffer[3] = 0x00;
			cmd_buffer[4] = 0x02;
			spi_master_transfer(cmd_buffer,&resp_buffer[0],  0x01);

		   /**********************************************************
			************** READ, EXPECT 0xA1 *************************
			*********************************************************/ 
			cmd_buffer[0] = SPI_READ_CMD;
			cmd_buffer[1] = 0xD4;
			cmd_buffer[2] = 0x00;
			cmd_buffer[3] = 0x00;
			spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);		
			
			
			if(retry > 10)
			{
				printf("\n\r Comm Error: Could Not obtain Locality ...\r\n");
				return;
			}
			retry++;
			HAL_Delay(10);
			
		} while(resp_buffer[0] != 0xA1);

	}
		
//	firstTime = false;
//}

RESEND_COMMAND:

/***************************************************************
	Write 0x40 to address FED40018 (Status Reg-commandReady)
 ***************************************************************/ 
	cmd_buffer[0] = SPI_WRITE_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x18;
	cmd_buffer[4] = 0x40;
	spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);

/*******************************************************************
	Read address FED40018-> Should return C8 (commandReady/stsValid)
 *******************************************************************/ 

	retry = 0;
	while(1)
	{
		
		cmd_buffer[0] = SPI_READ_CMD;
		cmd_buffer[1] = 0xD4;
		cmd_buffer[2] = 0x00;
		cmd_buffer[3] = 0x18;
		spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);

		/* expected results received */
		if((resp_buffer[0]&CMDRDY_MASK) == 0xC8)
			break;

		if(retry > MAX_RETRIES)
		{	
			printf("\n\r Comm Error: MASK 0xC8 (commandReady/stsValid) \
					\r\n             was never received\r\n");
			return;			
		}
		retry++;
		HAL_Delay(100);
		
	}

/**********************************************************
	Next steps are for command communication,...
	Write the following command sequence to the FIFO buffer 
	address (FED40024)
*********************************************************/ 

	memmove(&xferBuf[4],&xferBuf[0],numBytes);
	xferBuf[0] = SPI_WRITE_CMD;
	xferBuf[1] = 0xD4;
	xferBuf[2] = 0x00;
	xferBuf[3] = 0x24;
 
	if(spi_master_transfer(&xferBuf[0],&xferBuf[0],  numBytes) != TRANSFER_SUCCESS)
	{
		printf("\r\n COMM ERROR: During transfer to FED40024 ... \r\n");
		if(flush_tpm())
		{
			goto RESEND_COMMAND;
		}
	}




/**********************************************************
	Write 0x20 to  address FED40018, tpmGo bit 
*********************************************************/ 
retry_tpmGo = 0;
RETRY_TPMGO:

	cmd_buffer[0] = SPI_WRITE_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x18;
	cmd_buffer[4] = 0x20;
	spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);

/*************************************************************
	Poll address FED40018 after the command is sent to until
	the value 0x90(dataAvailable) is read.	
**************************************************************/ 
	retry		= 0;
	while(1)
	{

		cmd_buffer[0] = SPI_READ_CMD;
		cmd_buffer[1] = 0xD4;
		cmd_buffer[2] = 0x00;
		cmd_buffer[3] = 0x18;
		spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);
			
		/* expected results received */
		if((resp_buffer[0]&DATARDY_MASK) == 0x90)
			break;
		
		/* error, */
		if(retry > MAX_RETRIES)
		{	
			
			printf("\n\r Comm Error: MASK 0x90 (dataAvailable for reading) \
					\r\n             was never received\r\n");			
			
			if(retry_tpmGo < 2)
			{
				retry_tpmGo++;
				goto RETRY_TPMGO;
			}
			else
				return;
		}

		retry++;			
		HAL_Delay(100);
		
	}

/**********************************************************
	Read FIFO (address FED40024) for return data from TPM.
 *********************************************************/ 
	cmd_buffer[0] = SPI_READ_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x24;
	spi_master_transfer(&cmd_buffer[0],&xferBuf[0],  TPM_HEADER_SIZE);
	
	if( (numBytes > 1023) || (numBytes < 10) )
	{
		/* somethings wrong!! */
		printf("bad paramSize(%d)\r\n",numBytes);
		return;
	}

	/* show what we got */
	if(wantLog == getLog)
	{
		printf("\r\nfrom TPM:");
		dumpXferBuf();
	}
	
/**********************************************************
	Write 0x40 to address FED40018 (Status Reg-commandReady)
 *********************************************************/ 
	cmd_buffer[0] = SPI_WRITE_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x18;
	cmd_buffer[4] = 0x40;
	spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);
	
	/* send command 2x */
	cmd_buffer[0] = SPI_WRITE_CMD;
	cmd_buffer[1] = 0xD4;
	cmd_buffer[2] = 0x00;
	cmd_buffer[3] = 0x18;
	cmd_buffer[4] = 0x40;
	spi_master_transfer(&cmd_buffer[0],&resp_buffer[0],  0x01);
	
	
#endif

}
//-----------------------------------------------------------------------------
