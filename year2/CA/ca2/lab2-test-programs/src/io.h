#ifdef __riscv

/* The binaries we produce for the RISC-V emulator have no access to
   stdio so we implement the two functions we need using the UART */

inline int putchar(int c)
{
  //Address of the UART from serial.cc
  volatile char *debugout = (char*)0x200;

  *debugout = c;
  return 0;
}

static inline void puts(const char *str)
{
  for (const char *s = str; *s != '\0'; s++)
    putchar(*s);
  putchar('\n');
}

#else //not __riscv

//We assume all other platforms will have an io library
#include <stdio.h>

#endif
