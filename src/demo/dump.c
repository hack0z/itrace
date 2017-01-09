/*!The iOS Trace Utility
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
 * @file        dump.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <stdio.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t test2(int a, int b)
{
}
static tb_void_t test(int a, int b)
{
	tb_void_t (*f)(int, int) = &test2;
	f(a, b);
}
#if 0
static tb_void_t test3(tb_size_t x0, tb_size_t x1, tb_size_t x2, tb_size_t x3, tb_size_t x4, tb_size_t x5, tb_size_t x6, tb_size_t x7, tb_size_t x8, tb_size_t x9, tb_size_t x10, tb_size_t x11, tb_size_t x12, tb_size_t x13, tb_size_t x14, tb_size_t x15, tb_size_t x16)
{
}
#endif
static tb_void_t test4(tb_size_t x0, tb_size_t x1, tb_size_t x2, tb_size_t x3, tb_size_t x4, tb_size_t x5, tb_size_t x6, tb_size_t x7, tb_size_t x8, tb_size_t x9, ...)
{
	tb_va_list_t va;
	tb_va_start(va, x9);
	tb_trace_i("x0:%lx", x0);
	tb_trace_i("x1:%lx", x1);
	tb_trace_i("x2:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("x3:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("x4:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("x5:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("x6:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("x7:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("x8:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("s9:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("s10:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("s11:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("s12:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("s13:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("s14:%p", tb_va_arg(va, tb_size_t));
	tb_trace_i("s15:%p", tb_va_arg(va, tb_size_t));
	tb_va_end(va);
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t const** argv)
{
//test3(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
test4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
#if defined(TB_ARCH_ARM)
#ifdef TB_ARCH_ARM64
	__tb_asm__ __tb_volatile__("nop");
	__tb_asm__ __tb_volatile__("str x0, [sp, #-8]!");
	__tb_asm__ __tb_volatile__("stp x0, x1, [sp, #-16]!");

	__tb_asm__ __tb_volatile__("nop");
__tb_asm__ __tb_volatile__(".byte 0xe0");
__tb_asm__ __tb_volatile__(".byte 0x7");
__tb_asm__ __tb_volatile__(".byte 0xbf");
__tb_asm__ __tb_volatile__(".byte 0xa9");
__tb_asm__ __tb_volatile__(".byte 0xfe");
__tb_asm__ __tb_volatile__(".byte 0x8f");
__tb_asm__ __tb_volatile__(".byte 0x1f");
__tb_asm__ __tb_volatile__(".byte 0xf8");
__tb_asm__ __tb_volatile__(".byte 0x50");
__tb_asm__ __tb_volatile__(".byte 0x1");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x58");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x2");
__tb_asm__ __tb_volatile__(".byte 0x3f");
__tb_asm__ __tb_volatile__(".byte 0xd6");
__tb_asm__ __tb_volatile__(".byte 0xe0");
__tb_asm__ __tb_volatile__(".byte 0x7");
__tb_asm__ __tb_volatile__(".byte 0xc1");
__tb_asm__ __tb_volatile__(".byte 0xa8");
__tb_asm__ __tb_volatile__(".byte 0xe0");
__tb_asm__ __tb_volatile__(".byte 0x7");
__tb_asm__ __tb_volatile__(".byte 0xc1");
__tb_asm__ __tb_volatile__(".byte 0xa8");
__tb_asm__ __tb_volatile__(".byte 0xe0");
__tb_asm__ __tb_volatile__(".byte 0x7");
__tb_asm__ __tb_volatile__(".byte 0xc1");
__tb_asm__ __tb_volatile__(".byte 0xa8");
__tb_asm__ __tb_volatile__(".byte 0xe0");
__tb_asm__ __tb_volatile__(".byte 0x7");
__tb_asm__ __tb_volatile__(".byte 0xc1");
__tb_asm__ __tb_volatile__(".byte 0xa8");
__tb_asm__ __tb_volatile__(".byte 0x50");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x58");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x2");
__tb_asm__ __tb_volatile__(".byte 0x1f");
__tb_asm__ __tb_volatile__(".byte 0xd6");
__tb_asm__ __tb_volatile__(".byte 0xb4");
__tb_asm__ __tb_volatile__(".byte 0x50");
__tb_asm__ __tb_volatile__(".byte 0x2d");
__tb_asm__ __tb_volatile__(".byte 0x96");
__tb_asm__ __tb_volatile__(".byte 0x1");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0xb8");
__tb_asm__ __tb_volatile__(".byte 0xdb");
__tb_asm__ __tb_volatile__(".byte 0x5b");
__tb_asm__ __tb_volatile__(".byte 0x5");
__tb_asm__ __tb_volatile__(".byte 0x1");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0x0");
__tb_asm__ __tb_volatile__(".byte 0xe0");
__tb_asm__ __tb_volatile__(".byte 0x7");
__tb_asm__ __tb_volatile__(".byte 0xbf");
__tb_asm__ __tb_volatile__(".byte 0xa9");
__tb_asm__ __tb_volatile__(".byte 0xfe");
__tb_asm__ __tb_volatile__(".byte 0x8f");
__tb_asm__ __tb_volatile__(".byte 0x1f");
__tb_asm__ __tb_volatile__(".byte 0xf8");

	// substrate
#if 1
	// fopen
	// ldr x16, 0x8
	__tb_asm__ __tb_volatile__(".byte 0x50");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x58");

	// br x16
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x2");
	__tb_asm__ __tb_volatile__(".byte 0x1f");
	__tb_asm__ __tb_volatile__(".byte 0xd6");

	// function address, fopen_new: 0x01071adfd4
	__tb_asm__ __tb_volatile__(".byte 0xd4");
	__tb_asm__ __tb_volatile__(".byte 0xdf");
	__tb_asm__ __tb_volatile__(".byte 0x1a");
	__tb_asm__ __tb_volatile__(".byte 0x7");
	__tb_asm__ __tb_volatile__(".byte 0x1");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x0");

	__tb_asm__ __tb_volatile__("nop");
	__tb_asm__ __tb_volatile__("nop");
	__tb_asm__ __tb_volatile__("nop");

	// fopen_old
	// stp fp, lr, [sp, #-16]!
	__tb_asm__ __tb_volatile__(".byte 0xfd");
	__tb_asm__ __tb_volatile__(".byte 0x7b");
	__tb_asm__ __tb_volatile__(".byte 0xbf");
	__tb_asm__ __tb_volatile__(".byte 0xa9");

	// add fp, sp, 0
	__tb_asm__ __tb_volatile__(".byte 0xfd");
	__tb_asm__ __tb_volatile__(".byte 0x3");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x91");

	// stp x20, x19, [sp, #-16]!
	__tb_asm__ __tb_volatile__(".byte 0xf4");
	__tb_asm__ __tb_volatile__(".byte 0x4f");
	__tb_asm__ __tb_volatile__(".byte 0xbf");
	__tb_asm__ __tb_volatile__(".byte 0xa9");

	// stp x22, x21, [sp, #-16]!
	__tb_asm__ __tb_volatile__(".byte 0xf6");
	__tb_asm__ __tb_volatile__(".byte 0x57");
	__tb_asm__ __tb_volatile__(".byte 0xbf");
	__tb_asm__ __tb_volatile__(".byte 0xa9");

	// ldr x16, 0x8
	__tb_asm__ __tb_volatile__(".byte 0x50");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x58");

	// br x16
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x2");
	__tb_asm__ __tb_volatile__(".byte 0x1f");
	__tb_asm__ __tb_volatile__(".byte 0xd6");

	// function address: fopen: 0x019acb6798
	__tb_asm__ __tb_volatile__(".byte 0x98");
	__tb_asm__ __tb_volatile__(".byte 0x67");
	__tb_asm__ __tb_volatile__(".byte 0xcb");
	__tb_asm__ __tb_volatile__(".byte 0x9a");
	__tb_asm__ __tb_volatile__(".byte 0x1");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x0");
	__tb_asm__ __tb_volatile__(".byte 0x0");
#endif

	// ldr x16, 0x8
	__tb_asm__ __tb_volatile__(".byte 0x50");
	__tb_asm__ __tb_volatile__(".byte 0x00");
	__tb_asm__ __tb_volatile__(".byte 0x00");
	__tb_asm__ __tb_volatile__(".byte 0x58");

	// br x16
	__tb_asm__ __tb_volatile__(".byte 0x00");
	__tb_asm__ __tb_volatile__(".byte 0x02");
	__tb_asm__ __tb_volatile__(".byte 0x1f");
	__tb_asm__ __tb_volatile__(".byte 0xd6");

	__tb_asm__ __tb_volatile__("blr x9");
	__tb_asm__ __tb_volatile__("blr lr");

	__tb_asm__ __tb_volatile__("movz x0, #65535");
	__tb_asm__ __tb_volatile__("movk x0, #65535, lsl #16");
	__tb_asm__ __tb_volatile__("movk x0, #65535, lsl #32");
	__tb_asm__ __tb_volatile__("movk x0, #65535, lsl #48");

	__tb_asm__ __tb_volatile__("ldr x0, [x0, #-4]");
	__tb_asm__ __tb_volatile__("ldr x0, [x0, #4]");
	__tb_asm__ __tb_volatile__("ldr x0, [sp, #8]");
	__tb_asm__ __tb_volatile__("ret x0");

	__tb_asm__ __tb_volatile__("stp x0, x1, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x2, x3, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x4, x5, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x6, x7, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x8, x9, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x10, x11, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x12, x13, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x14, x15, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x16, x17, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x18, x19, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x20, x21, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x22, x23, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x24, x25, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x26, x27, [sp, #-16]!");
	__tb_asm__ __tb_volatile__("stp x28, lr, [sp, #-16]!");

	__tb_asm__ __tb_volatile__("ldp x0, x1, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x2, x3, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x4, x5, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x6, x7, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x8, x9, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x10, x11, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x12, x13, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x14, x15, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x16, x17, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x18, x19, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x20, x21, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x22, x23, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x24, x25, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x26, x27, [sp], #16");
	__tb_asm__ __tb_volatile__("ldp x28, lr, [sp], #16");

#else
	__tb_asm__ __tb_volatile__("nop");
	__tb_asm__ __tb_volatile__("push {r0-r9, lr}");
	__tb_asm__ __tb_volatile__("bx r1");
	__tb_asm__ __tb_volatile__("ldr pc, [pc, #4]");
	__tb_asm__ __tb_volatile__("pop {r0-r9, lr}");
#endif

#elif defined(TB_ARCH_x86)

	__tb_asm__ __tb_volatile__("nop");

	__tb_asm__ __tb_volatile__("pushal");
	__tb_asm__ __tb_volatile__("pushfl");
	__tb_asm__ __tb_volatile__("push %eax");
	__tb_asm__ __tb_volatile__("push %ebx");
	__tb_asm__ __tb_volatile__("push %ecx");
	__tb_asm__ __tb_volatile__("push %edx");
	__tb_asm__ __tb_volatile__("push %edi");
	__tb_asm__ __tb_volatile__("push %esi");
	__tb_asm__ __tb_volatile__("push %ebp");
	__tb_asm__ __tb_volatile__("push $0x1");
	__tb_asm__ __tb_volatile__("push $0x2");
	__tb_asm__ __tb_volatile__("movl $0x12345678, %eax");
	__tb_asm__ __tb_volatile__("call %eax");
	__tb_asm__ __tb_volatile__("addl $0x8, %esp");
	__tb_asm__ __tb_volatile__("pop %eax");
	__tb_asm__ __tb_volatile__("popal");
	__tb_asm__ __tb_volatile__("popfl");

	__tb_asm__ __tb_volatile__("push $0x12345678");
	__tb_asm__ __tb_volatile__("ret");

#elif defined(TB_ARCH_x64)
	
	__tb_asm__ __tb_volatile__("nop");

//	__tb_asm__ __tb_volatile__("pushaq");
	__tb_asm__ __tb_volatile__("pushfq");
	__tb_asm__ __tb_volatile__("pushq %rax");
	__tb_asm__ __tb_volatile__("pushq %rbx");
	__tb_asm__ __tb_volatile__("pushq %rcx");
	__tb_asm__ __tb_volatile__("pushq %rdx");
	__tb_asm__ __tb_volatile__("pushq %rdi");
	__tb_asm__ __tb_volatile__("pushq %rsi");
	__tb_asm__ __tb_volatile__("pushq %rbp");
	__tb_asm__ __tb_volatile__("pushq %r8");
	__tb_asm__ __tb_volatile__("pushq %r9");
	__tb_asm__ __tb_volatile__("pushq %r10");
	__tb_asm__ __tb_volatile__("pushq %r11");
	__tb_asm__ __tb_volatile__("pushq %r12");
	__tb_asm__ __tb_volatile__("pushq %r13");
	__tb_asm__ __tb_volatile__("pushq %r14");
	__tb_asm__ __tb_volatile__("pushq %r15");
	__tb_asm__ __tb_volatile__("pushq $0x1");
	__tb_asm__ __tb_volatile__("pushq $0x22334455");
	__tb_asm__ __tb_volatile__("movq $0x1234567811223344, %rax");
	__tb_asm__ __tb_volatile__("movq $0x1234567811223344, %rbx");
//	__tb_asm__ __tb_volatile__("callq %rax");
//	__tb_asm__ __tb_volatile__("callq %rbx");
	__tb_asm__ __tb_volatile__("addq $0x8, %rsp");
	__tb_asm__ __tb_volatile__("popq %rbp");
	__tb_asm__ __tb_volatile__("popq %rax");
	__tb_asm__ __tb_volatile__("popq %rbx");
	__tb_asm__ __tb_volatile__("popq %rcx");
	__tb_asm__ __tb_volatile__("popq %rdx");
	__tb_asm__ __tb_volatile__("popq %rdi");
	__tb_asm__ __tb_volatile__("popq %rsi");
	__tb_asm__ __tb_volatile__("popq %r8");
	__tb_asm__ __tb_volatile__("popq %r9");
	__tb_asm__ __tb_volatile__("popq %r10");
	__tb_asm__ __tb_volatile__("popq %r11");
	__tb_asm__ __tb_volatile__("popq %r12");
	__tb_asm__ __tb_volatile__("popq %r13");
	__tb_asm__ __tb_volatile__("popq %r14");
	__tb_asm__ __tb_volatile__("popq %r15");
	__tb_asm__ __tb_volatile__("popfq");
//	__tb_asm__ __tb_volatile__("popaq");

//	__tb_asm__ __tb_volatile__("pushq $0x1234567811223344");
	__tb_asm__ __tb_volatile__("subq $0x8, %rsp");
	__tb_asm__ __tb_volatile__("movl $0x11223344, 0x4(%rsp)");
	__tb_asm__ __tb_volatile__("movl $0x12345678, (%rsp)");

	__tb_asm__ __tb_volatile__("movq $0x1234567811223344, %rdi");
	__tb_asm__ __tb_volatile__("movq $0x1234567811223344, %rsi");

	__tb_asm__ __tb_volatile__("ret");

#endif

	tb_used(test);

	return 0;
}
