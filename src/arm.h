#ifndef IT_ARM_H
#define IT_ARM_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros for arm
 */

// ldr rd, [rn, #im]
#define A$ldr_rd_$rn_im$(rd, rn, im) 	(0xe5100000 | ((im) < 0 ? 0 : 1 << 23) | ((rn) << 16) | ((rd) << 12) | abs(im))

// movw rd, im
#define A$movw_rd_im(rd, im) 			(0xe3000000 | (((im) & 0xf000) << 4) | ((rd) << 12) | ((im) & 0x0fff))

// movt rd, im
#define A$movt_rd_im(rd, im) 			(0xe3400000 | (((im) & 0xf000) << 4) | ((rd) << 12) | ((im) & 0x0fff))

// push {r0-r9, lr}
#define A$push$r0_r9_lr$ 				(0xe92d43ff)

// pop {r0-r9, lr}
#define A$pop$r0_r9_lr$ 				(0xe8bd43ff)

// blx rm
#define A$blx(rm) 						(0xe12fff30 | ((rm) & 0x0000000f))

// nop
#define A$nop 							(0xe1a00000)

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros for thumb
 */

// blx rm
#define T$blx(rm) 						(0x4780 | ((rm) << 3))

// bx rm
#define T$bx(rm) 						(0x4700 | ((rm) << 3))

// nop
#define T$nop 							(0x46c0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the arm register enum
typedef enum __A$r_e
{
 	A$r0 = 0
, 	A$r1
, 	A$r2
, 	A$r3
, 	A$r4
, 	A$r5
, 	A$r6
, 	A$r7
, 	A$r8
, 	A$r9
, 	A$r10
, 	A$r11
, 	A$r12
, 	A$r13
, 	A$r14
, 	A$r15
, 	A$sp = A$r13
, 	A$lr = A$r14
, 	A$pc = A$r15

}A$r_e;

// the thumb register enum
typedef enum __T$r_e
{
 	T$r0 = 0
, 	T$r1
, 	T$r2
, 	T$r3
, 	T$r4
, 	T$r5
, 	T$r6
, 	T$r7
, 	T$r8
, 	T$r9
, 	T$r10
, 	T$r11
, 	T$r12
, 	T$r13
, 	T$r14
, 	T$r15
, 	T$sp = T$r13
, 	T$lr = T$r14
, 	T$pc = T$r15

}T$r_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * __tb_inline__
 */
static __tb_inline__ tb_bool_t A$pcrel$r(tb_uint32_t ic)
{
    return (ic & 0x0c000000) == 0x04000000 && (ic & 0xf0000000) != 0xf0000000 && (ic & 0x000f0000) == 0x000f0000;
}
static __tb_inline__ tb_bool_t T$32bit$i(tb_uint16_t ic)
{
    return ((ic & 0xe000) == 0xe000 && (ic & 0x1800) != 0x0000);
}
static __tb_inline__ tb_bool_t T$pcrel$bl(tb_uint16_t *ic)
{
    return (ic[0] & 0xf800) == 0xf000 && (ic[1] & 0xf800) == 0xe800;
}
static __tb_inline__ tb_bool_t T$pcrel$ldr(tb_uint16_t ic)
{
    return (ic & 0xf800) == 0x4800;
}
static __tb_inline__ tb_bool_t T$pcrel$add(tb_uint16_t ic)
{
    return (ic & 0xff78) == 0x4478;
}
static __tb_inline__ tb_bool_t T$pcrel$ldrw(tb_uint16_t ic)
{
    return (ic & 0xff7f) == 0xf85f;
}


#endif


