/*
 * Header files for monitor comms
 */

#ifdef DEBUG
#define USERICHSTRINGS
#endif

char *bytetohex(uint8_t byte);
uint8_t hextobyte(char hex[2]);

char *bytetohex(uint8_t byte)
{
    char *c = "\0\0";
    uint8_t nybble0 = (byte >> 4) & 0x0F;
    uint8_t nybble1 = byte& 0x0F;
    if (nybble0 > 0x09)
        nybble0 += 0x37; // A-F
    else
        nybble0 += 0x30; // 0-9

    if (nybble1 > 0x09)
        nybble1 += 0x37; // A-F
    else
        nybble1 += 0x30; // 0-9
    c[0] = nybble0;
    c[1] = nybble1;
    return c;
}

// Finish me later
uint8_t hextobyte(char hex[2])
{
    uint8_t byte = 0x00;
    return byte;
}
