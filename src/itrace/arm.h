/*!The iOS Trace Utility
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2015 - 2017, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        arm.h
 *
 */
#ifndef IT_ARM_H
#define IT_ARM_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros for arm64
 */

// str xt, [xn, #im]!
#define A64$str_xt_$xn_im$(xt, xn, im)                  (0xf8000c00 | (((im) & 0x1ff) << 12) | (((xn) & 0x1f) << 5) | ((xt) & 0x1f))

// ldr xt, [xn], #im
#define A64$ldr_xt_$xn$_im(xt, xn, im)                  (0xf8400400 | (((im) & 0x1ff) << 12) | (((xn) & 0x1f) << 5) | ((xt) & 0x1f))

// stp xt1, xt2, [xn, #im]!
#define A64$stp_xt1_xt2_$xn_im$(xt1, xt2, xn, im)       (0xa9800000 | ((((im) >> 3) & 0x7f) << 15) | (((xt2) & 0x1f) << 10) | (((xn) & 0x1f) << 5) | ((xt1) & 0x1f))

// ldp xt1, xt2, [xn], #im
#define A64$ldp_xt1_xt2_$xn$_im(xt1, xt2, xn, im)       (0xa8c00000 | ((((im) >> 3) & 0x7f) << 15) | (((xt2) & 0x1f) << 10) | (((xn) & 0x1f) << 5) | ((xt1) & 0x1f))

// movz xd, #im, lsl #shift
#define A64$movz_xd_im(xd, im, shift)                   (0xd2800000 | (((shift) >> 4) << 21) | (((im) & 0xffff) << 5) | ((xd) & 0x1f))

// movk xd, #im, lsl #shift
#define A64$movk_xd_im(xd, im, shift)                   (0xf2800000 | (((shift) >> 4) << 21) | (((im) & 0xffff) << 5) | ((xd) & 0x1f))

// br xn
#define A64$br(xn)                                      (0xd61f0000 | (((xn) & 0x1f) << 5))

// blr xn
#define A64$blr(xn)                                     (0xd63f0000 | (((xn) & 0x1f) << 5))

// ret xn
#define A64$ret(xn)                                     (0xd65f0000 | (((xn) & 0x1f) << 5))

// ldr xt, #im
#define A64$ldr_xt_im(xt, im)                           (0x58000000 | ((((im) >> 2) & 0xfffff) << 5) | ((xt) & 0x1f))

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros for arm
 */

// ldr rd, [rn, #im]
#define A$ldr_rd_$rn_im$(rd, rn, im)    (0xe5100000 | ((im) < 0 ? 0 : 1 << 23) | ((rn) << 16) | ((rd) << 12) | abs(im))

// movw rd, #im
#define A$movw_rd_im(rd, im)            (0xe3000000 | (((im) & 0xf000) << 4) | ((rd) << 12) | ((im) & 0x0fff))

// movt rd, #im
#define A$movt_rd_im(rd, im)            (0xe3400000 | (((im) & 0xf000) << 4) | ((rd) << 12) | ((im) & 0x0fff))

// push {r0-r9, lr}
#define A$push$r0_r9_lr$                (0xe92d43ff)

// pop {r0-r9, lr}
#define A$pop$r0_r9_lr$                 (0xe8bd43ff)

// blx rm
#define A$blx(rm)                       (0xe12fff30 | ((rm) & 0x0000000f))

// nop
#define A$nop                           (0xe1a00000)

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros for thumb
 */

// blx rm
#define T$blx(rm)                       (0x4780 | ((rm) << 3))

// bx rm
#define T$bx(rm)                        (0x4700 | ((rm) << 3))

// nop
#define T$nop                           (0x46c0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the arm64 register enum
typedef enum __A64$x_e
{
    A64$x0 = 0
,   A64$x1
,   A64$x2
,   A64$x3
,   A64$x4
,   A64$x5
,   A64$x6
,   A64$x7
,   A64$x8
,   A64$x9
,   A64$x10
,   A64$x11
,   A64$x12
,   A64$x13
,   A64$x14
,   A64$x15
,   A64$x16
,   A64$x17
,   A64$x18
,   A64$x19
,   A64$x20
,   A64$x21
,   A64$x22
,   A64$x23
,   A64$x24
,   A64$x25
,   A64$x26
,   A64$x27
,   A64$x28
,   A64$x29
,   A64$x30
,   A64$x31
,   A64$x32
,   A64$fp  = A64$x29
,   A64$lr  = A64$x30
,   A64$sp  = A64$x31

}A64$x_e;

// the arm register enum
typedef enum __A$r_e
{
    A$r0 = 0
,   A$r1
,   A$r2
,   A$r3
,   A$r4
,   A$r5
,   A$r6
,   A$r7
,   A$r8
,   A$r9
,   A$r10
,   A$r11
,   A$r12
,   A$r13
,   A$r14
,   A$r15
,   A$sp = A$r13
,   A$lr = A$r14
,   A$pc = A$r15

}A$r_e;

// the thumb register enum
typedef enum __T$r_e
{
    T$r0 = 0
,   T$r1
,   T$r2
,   T$r3
,   T$r4
,   T$r5
,   T$r6
,   T$r7
,   T$r8
,   T$r9
,   T$r10
,   T$r11
,   T$r12
,   T$r13
,   T$r14
,   T$r15
,   T$sp = T$r13
,   T$lr = T$r14
,   T$pc = T$r15

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


