/*
 * Header files for monitor comms
 */

#ifdef DEBUG
#define USERICHSTRINGS
#endif

char *bytetohex(uint8_t byte);
uint8_t hextobyte(char hex[2]);
void dumpROM(void);
