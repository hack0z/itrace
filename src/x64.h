#ifndef IT_x64_H
#define IT_x64_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// push$im
#define x64$push$im(p, m) 					do { *(p)++ = 0x68; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)
//#define x64$push$im(p, m) 					do { *(p)++ = 0x48; *(p)++ = 0xb8; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; *(p)++ = ((m) >> 32) & 0xff; *(p)++ = ((m) >> 40) & 0xff; *(p)++ = ((m) >> 48) & 0xff; *(p)++ = ((m) >> 56) & 0xff; } while (0)

// push rax
#define x64$push$rax(p) 					do { *(p)++ = 0x50; } while (0)

// pop rax
#define x64$pop$rax(p) 						do { *(p)++ = 0x58; } while (0)

// mov$rax$im
#define x64$mov$rax$im(p, m) 				do { *(p)++ = 0x48; *(p)++ = 0xb8; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; *(p)++ = ((m) >> 32) & 0xff; *(p)++ = ((m) >> 40) & 0xff; *(p)++ = ((m) >> 48) & 0xff; *(p)++ = ((m) >> 56) & 0xff; } while (0)

// call rax
#define x64$call$rax(p) 					do { *(p)++ = 0xff; *(p)++ = 0xd0; } while (0)

// add rsp, im
#define x64$add$rsp$im(p, m) 				do { *(p)++ = 0x48; *(p)++ = 0x83; *(p)++ = 0xc4; *(p)++ = (m); } while (0)

// ret
#define x64$ret(p) 							do { *(p)++ = 0xc3; } while (0)

// nop
#define x64$nop(p) 							do { *(p)++ = 0x90; } while (0)


/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */



#endif


