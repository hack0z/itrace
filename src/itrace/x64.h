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
#define x64$push$im(p, m)                   do { *(p)++ = 0x68; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)

// pushf
#define x64$pushf(p)                        do { *(p)++ = 0x9c; } while (0)

// push rax
#define x64$push$rax(p)                     do { *(p)++ = 0x50; } while (0)

// push rbx
#define x64$push$rbx(p)                     do { *(p)++ = 0x53; } while (0)

// push rcx
#define x64$push$rcx(p)                     do { *(p)++ = 0x51; } while (0)

// push rdx
#define x64$push$rdx(p)                     do { *(p)++ = 0x52; } while (0)

// push rdi
#define x64$push$rdi(p)                     do { *(p)++ = 0x57; } while (0)

// push rsi
#define x64$push$rsi(p)                     do { *(p)++ = 0x56; } while (0)

// push rbp
#define x64$push$rbp(p)                     do { *(p)++ = 0x55; } while (0)

// push r8
#define x64$push$r8(p)                      do { *(p)++ = 0x41; *(p)++ = 0x50; } while (0)

// push r9
#define x64$push$r9(p)                      do { *(p)++ = 0x41; *(p)++ = 0x51; } while (0)

// push r10
#define x64$push$r10(p)                     do { *(p)++ = 0x41; *(p)++ = 0x52; } while (0)

// push r11
#define x64$push$r11(p)                     do { *(p)++ = 0x41; *(p)++ = 0x53; } while (0)

// push r12
#define x64$push$r12(p)                     do { *(p)++ = 0x41; *(p)++ = 0x54; } while (0)

// push r13
#define x64$push$r13(p)                     do { *(p)++ = 0x41; *(p)++ = 0x55; } while (0)

// push r14
#define x64$push$r14(p)                     do { *(p)++ = 0x41; *(p)++ = 0x56; } while (0)

// push r15
#define x64$push$r15(p)                     do { *(p)++ = 0x41; *(p)++ = 0x57; } while (0)

// popf
#define x64$popf(p)                         do { *(p)++ = 0x9d; } while (0)

// pop rbp
#define x64$pop$rbp(p)                      do { *(p)++ = 0x5d; } while (0)

// pop rax
#define x64$pop$rax(p)                      do { *(p)++ = 0x58; } while (0)

// pop rbx
#define x64$pop$rbx(p)                      do { *(p)++ = 0x5b; } while (0)

// pop rcx
#define x64$pop$rcx(p)                      do { *(p)++ = 0x59; } while (0)

// pop rdx
#define x64$pop$rdx(p)                      do { *(p)++ = 0x5a; } while (0)

// pop rdi
#define x64$pop$rdi(p)                      do { *(p)++ = 0x5f; } while (0)

// pop rsi
#define x64$pop$rsi(p)                      do { *(p)++ = 0x5e; } while (0)

// pop r8
#define x64$pop$r8(p)                       do { *(p)++ = 0x41; *(p)++ = 0x58; } while (0)

// pop r9
#define x64$pop$r9(p)                       do { *(p)++ = 0x41; *(p)++ = 0x59; } while (0)

// pop r10
#define x64$pop$r10(p)                      do { *(p)++ = 0x41; *(p)++ = 0x5a; } while (0)

// pop r11
#define x64$pop$r11(p)                      do { *(p)++ = 0x41; *(p)++ = 0x5b; } while (0)

// pop r12
#define x64$pop$r12(p)                      do { *(p)++ = 0x41; *(p)++ = 0x5c; } while (0)

// pop r13
#define x64$pop$r13(p)                      do { *(p)++ = 0x41; *(p)++ = 0x5d; } while (0)

// pop r14
#define x64$pop$r14(p)                      do { *(p)++ = 0x41; *(p)++ = 0x5e; } while (0)

// pop r15
#define x64$pop$r15(p)                      do { *(p)++ = 0x41; *(p)++ = 0x5f; } while (0)

// mov$rax$im
#define x64$mov$rax$im(p, m)                do { *(p)++ = 0x48; *(p)++ = 0xb8; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; *(p)++ = ((m) >> 32) & 0xff; *(p)++ = ((m) >> 40) & 0xff; *(p)++ = ((m) >> 48) & 0xff; *(p)++ = ((m) >> 56) & 0xff; } while (0)

// mov$rbx$im
#define x64$mov$rbx$im(p, m)                do { *(p)++ = 0x48; *(p)++ = 0xbb; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; *(p)++ = ((m) >> 32) & 0xff; *(p)++ = ((m) >> 40) & 0xff; *(p)++ = ((m) >> 48) & 0xff; *(p)++ = ((m) >> 56) & 0xff; } while (0)

// mov$rdi$im
#define x64$mov$rdi$im(p, m)                do { *(p)++ = 0x48; *(p)++ = 0xbf; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; *(p)++ = ((m) >> 32) & 0xff; *(p)++ = ((m) >> 40) & 0xff; *(p)++ = ((m) >> 48) & 0xff; *(p)++ = ((m) >> 56) & 0xff; } while (0)

// mov$rsi$im
#define x64$mov$rsi$im(p, m)                do { *(p)++ = 0x48; *(p)++ = 0xbe; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; *(p)++ = ((m) >> 32) & 0xff; *(p)++ = ((m) >> 40) & 0xff; *(p)++ = ((m) >> 48) & 0xff; *(p)++ = ((m) >> 56) & 0xff; } while (0)

// mov$rsp$im
#define x64$mov$rsp$im(p, m)                do { *(p)++ = 0xc7; *(p)++ = 0x04; *(p)++ = 0x24; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)

// mov$rsp4$im
#define x64$mov$rsp4$im(p, m)               do { *(p)++ = 0xc7; *(p)++ = 0x44; *(p)++ = 0x24; *(p)++ = 0x04; *(p)++ = (m) & 0xff; *(p)++ = ((m) >> 8) & 0xff; *(p)++ = ((m) >> 16) & 0xff; *(p)++ = ((m) >> 24) & 0xff; } while (0)

// call rax
#define x64$call$rax(p)                     do { *(p)++ = 0xff; *(p)++ = 0xd0; } while (0)

// call rbx
#define x64$call$rbx(p)                     do { *(p)++ = 0xff; *(p)++ = 0xd3; } while (0)

// add rsp, im
#define x64$add$rsp$im(p, m)                do { *(p)++ = 0x48; *(p)++ = 0x83; *(p)++ = 0xc4; *(p)++ = (m); } while (0)

// sub rsp, im
#define x64$sub$rsp$im(p, m)                do { *(p)++ = 0x48; *(p)++ = 0x83; *(p)++ = 0xec; *(p)++ = (m); } while (0)

// ret
#define x64$ret(p)                          do { *(p)++ = 0xc3; } while (0)

// nop
#define x64$nop(p)                          do { *(p)++ = 0x90; } while (0)


/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */



#endif


