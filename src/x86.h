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
#define x86$push$im(p, m) 					do { *(p)++ = 0x68; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)

// pusha
#define x86$pusha(p) 						do { *(p)++ = 0x60; } while (0)

// popa
#define x86$popa(p) 						do { *(p)++ = 0x61; } while (0)

// push eax
#define x86$push$eax(p) 					do { *(p)++ = 0x50; } while (0)

// pop eax
#define x86$pop$eax(p) 						do { *(p)++ = 0x58; } while (0)

// mov$eax$im
#define x86$mov$eax$im(p, m) 				do { *(p)++ = 0xb8; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)

// call eax
#define x86$call$eax(p) 					do { *(p)++ = 0xff; *(p)++ = 0xd0; } while (0)

// add esp, im
#define x86$add$esp$im(p, m) 				do { *(p)++ = 0x83; *(p)++ = 0xc4; *(p)++ = (m); } while (0)

// ret
#define x86$ret(p) 							do { *(p)++ = 0xc3; } while (0)

// nop
#define x86$nop(p) 							do { *(p)++ = 0x90; } while (0)


/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */



#endif


