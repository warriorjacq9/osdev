#include <drivers/fat.h>
#include <drivers/ata_pio.h>

void trim(char *c, int max)
{
    int i = 0;
    while(*c != ' ' && i++ < max)
    {
        c++;
    }
    if(*c == ' ') *c = 0;
}

uint16_t read16(uint8_t *buff, uint16_t offset)
{
    uint8_t *ubuff = buff + offset;
    return ubuff[1] << 8 | ubuff[0];
}

uint32_t read32(uint8_t *buff, uint16_t offset)
{
    uint8_t *ubuff = buff + offset;
    return
        ((ubuff[3] << 24) & 0xFF000000) |
        ((ubuff[2] << 16) & 0x00FF0000) |
        ((ubuff[1] << 8) & 0x0000FF00) |
        (ubuff[0] & 0x000000FF);
}
