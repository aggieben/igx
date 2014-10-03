#ifndef __IGX__
#define __IGX__

#define _IGX_VER 0
#define _IGX_REV 0
#define _IGX_BLD 108


void _start(long * mb_info);

#define NULL 0

typedef unsigned long ulong;
typedef unsigned char byte;

typedef struct cpuid
{
  ulong max_eax;
  /* vendor string */
  unsigned long int vs[3]; 
  byte type;
  byte family;
  byte model;
  byte stepping;
} cpuid;

extern ulong _end;

#endif
