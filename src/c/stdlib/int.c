#include "../../../headers/fsl.h"

public i32 is_i32_valid(i32 num)
{
    return (num < -0x80000000 || num > 0x7FFFFFFF);
}

public i32 count_int_digits(i32 num) {
    int n = num;
    if(n == 0) return 1;
    if(n < 0) n = -n;

    int count = 0;
    while(n > 0) {
        n /= 10;
        count++;
    }

    return count;
}

public int str_to_int(const char *s)
{
    int sign = 1;
    int result = 0;

    // handle optional sign
    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    // parse digits
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s - '0');
        s++;
    }

    return result * sign;
}
