/*
 * Header files bootloader
 */

void get512block(uint8_t *buffer, uint16_t baseaddress);
void get1024block(uint8_t *buffer, uint16_t baseaddress);

void (*run_firmware)(void) = 0x00000;

