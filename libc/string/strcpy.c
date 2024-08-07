#include <string.h>

char *strcpy(char *dest, const char *src)
{
	int i = 0;
    while (1)
    {
        dest[i] = src[i];

        if (dest[i] == '\0')
        {
            break;
        }

        i++;
    }
    return dest;
}