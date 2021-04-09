/*
 * Header files bootloader
 */

uint16_t *get512block(uint16_t baseaddress);
void (*run_firmware)(void) = 0x0000;

