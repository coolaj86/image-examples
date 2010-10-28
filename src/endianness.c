// http://www.linuxforums.org/forum/programming-scripting/63464-finding-out-systems-endianness.html
#include <stdio.h>

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN    1

typedef union
{
        int i;
        char c[4];
}u;

int machineEndianness()
{
  int i = 1;
  char *p = (char *) &i;
  // if (p[0] == 1) // Lowest address contains the least significant byte
  return (p[0] == 1) ? LITTLE_ENDIAN : BIG_ENDIAN;
}

visualMachineEndianness()
{
  u temp;
  temp.i = 0x12345678;

  printf("%x\n", temp.i);
  printf("%x %x %x %x\n", temp.c[0], temp.c[1], temp.c[2], temp.c[3]);
  //If you get the output of the second line as "78 56 34 12", the machine is little endian.
  //Instead if the output is "12 34 56 78", it's a big endian machine.
}
