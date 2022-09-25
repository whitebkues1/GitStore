#include "main.h"

void translate_address(uint32_t ul_addr, uint16_t *pus_page, uint16_t *pus_offset);

void compute_address(uint16_t us_page, uint16_t us_offset, uint32_t *pul_addr);

uint32_t flash_write(uint32_t ul_address, const void *p_buffer, uint32_t ul_size, uint32_t ul_erase_flag);
