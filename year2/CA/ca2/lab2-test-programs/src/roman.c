#include "io.h"

static struct {
    int base;
    char repr;
} roman_table[] = {
    {    1000,   'M' },
    {     500,   'D' },
    {     100,   'C' },
    {      50,   'L' },
    {      10,   'X' },
    {       5,   'V' },
    {       1,   'I' },
    {       0,     0 }
};

void print_roman(int x)
{
    int i;

    for (i = 0; roman_table[i].base != 0; ++i)
    {
        while (x >= roman_table[i].base)
        {
            putchar(roman_table[i].repr);
            x = x - roman_table[i].base;
        }
    }

    putchar('\n');
}
