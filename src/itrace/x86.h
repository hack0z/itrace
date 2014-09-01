#ifndef IT_x86_H
#define IT_x86_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// push$im
#define x86$push$im(p, m)                   do { *(p)++ = 0x68; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)

// pushf
#define x86$pushf(p)                        do { *(p)++ = 0x9c; } while (0)

// popf
#define x86$popf(p)                         do { *(p)++ = 0x9d; } while (0)

// pusha
#define x86$pusha(p)                        do { *(p)++ = 0x60; } while (0)

// popa
#define x86$popa(p)                         do { *(p)++ = 0x61; } while (0)

// push eax
#define x86$push$eax(p)                     do { *(p)++ = 0x50; } while (0)

// push ebx
#define x86$push$ebx(p)                     do { *(p)++ = 0x53; } while (0)

// push ecx
#define x86$push$ecx(p)                     do { *(p)++ = 0x51; } while (0)

// push edx
#define x86$push$edx(p)                     do { *(p)++ = 0x52; } while (0)

// push edi
#define x86$push$edi(p)                     do { *(p)++ = 0x57; } while (0)

// push esi
#define x86$push$esi(p)                     do { *(p)++ = 0x56; } while (0)

// pop eax
#define x86$pop$eax(p)                      do { *(p)++ = 0x58; } while (0)

// pop ebx
#define x86$pop$ebx(p)                      do { *(p)++ = 0x5b; } while (0)

// pop ecx
#define x86$pop$ecx(p)                      do { *(p)++ = 0x59; } while (0)

// pop edx
#define x86$pop$edx(p)                      do { *(p)++ = 0x5a; } while (0)

// pop edi
#define x86$pop$edi(p)                      do { *(p)++ = 0x5f; } while (0)

// pop esi
#define x86$pop$esi(p)                      do { *(p)++ = 0x5e; } while (0)

// mov$eax$im
#define x86$mov$eax$im(p, m)                do { *(p)++ = 0xb8; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)

// call eax
#define x86$call$eax(p)                     do { *(p)++ = 0xff; *(p)++ = 0xd0; } while (0)

// add esp, im
#define x86$add$esp$im(p, m)                do { *(p)++ = 0x83; *(p)++ = 0xc4; *(p)++ = (m); } while (0)

// ret
#define x86$ret(p)                          do { *(p)++ = 0xc3; } while (0)

// nop
#define x86$nop(p)                          do { *(p)++ = 0x90; } while (0)


/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */



#endif


