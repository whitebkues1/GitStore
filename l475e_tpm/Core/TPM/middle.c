#include "middle.h"
#include "conf_board.h"
#include "usart.h"

void translate_address(uint32_t ul_addr, uint16_t *pus_page, uint16_t *pus_offset)
{
	uint16_t us_page;
	uint16_t us_offset;


	us_page = (ul_addr - FLASH_BASE) / FLASH_PAGE_SIZE;
	us_offset = (ul_addr - FLASH_BASE) % FLASH_PAGE_SIZE;

	if (pus_page) {
		*pus_page = us_page;
	}

	if (pus_offset) {
		*pus_offset = us_offset;
	}

}

void compute_address(uint16_t us_page, uint16_t us_offset, uint32_t *pul_addr)
{
	uint32_t ul_addr;

	ul_addr = FLASH_BASE + us_page * FLASH_PAGE_SIZE + us_offset;

	if (pul_addr != NULL) {
		*pul_addr = ul_addr;
	}
}

uint32_t flash_write(uint32_t ul_address, const void *p_buffer, uint32_t ul_size, uint32_t ul_erase_flag)
{
	uint16_t i = 0;
	uint64_t DATA_64[ul_size / sizeof(uint64_t)];
	memcpy(DATA_64, p_buffer, ul_size);

	for (i = 0; i < (ul_size / 8 + 1); i++)
	{
	    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ul_address, DATA_64[i]) == HAL_OK)
	    {
	    	ul_address = ul_address + 8;
	    }
	}

	return HAL_OK;
}
