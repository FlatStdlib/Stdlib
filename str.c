#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int str_startswith(const char *buffer, const char *needle)
{
    int len = strlen(buffer);
    int slen = strlen(needle);
    for(int i = 0; i < len; i++)
    {
        int chk = 0;
        for(int c = 0; c < slen; c++) {
            if(buffer[i + c] != needle[c])
                return 0;
            else
                chk = 1;
        }

        if(chk)
            return 1;
    }

    return 0;
}

#define TEST_STR "dick SHIT"

int main()
{
    printf("%d\n", str_startswith(TEST_STR, "dic"));
    return 0;
}