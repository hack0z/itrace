/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <stdio.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t test3(int x0, int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, long long x9, long long x10)
{
}
static tb_void_t test2(int a, int b)
{
}
static tb_void_t test(int a, int b)
{
	tb_void_t (*f)(int, int) = &test2;
	f(a, b);
}
#if 0
typedef enum __A64$x_e
{
 	A64$x0 = 0
, 	A64$x1
, 	A64$x2
, 	A64$x3
, 	A64$x4
, 	A64$x5
, 	A64$x6
, 	A64$x7
, 	A64$x8
, 	A64$x9
, 	A64$x10
, 	A64$x11
, 	A64$x12
, 	A64$x13
, 	A64$x14
, 	A64$x15
, 	A64$x16
, 	A64$x17
, 	A64$x18
, 	A64$x19
, 	A64$x20
, 	A64$x21
, 	A64$x22
, 	A64$x23
, 	A64$x24
, 	A64$x25
, 	A64$x26
, 	A64$x27
, 	A64$x28
, 	A64$x29
, 	A64$x30
, 	A64$x31
, 	A64$x32
, 	A64$fp 	= A64$x29
, 	A64$lr 	= A64$x30
, 	A64$sp 	= A64$x31
, 	A64$pc 	= A64$x32

}A64$x_e;
#define A64$ldur_xt_$xn_im$(xt, xn, im) 				(0xf8400000 | (((im) & 0x1ff) << 12) | (((xn) & 0x1f) << 5) | ((xt) & 0x1f))
#define A64$movk_xd_im(xd, im, shift) 					(0xf2800000 | (((shift) >> 4) << 21) | (((im) & 0xffff) << 5) | ((xd) & 0x1f))
#endif
/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t const** argv)
{
//	tb_trace_i("%x", A64$ldur_xt_$xn_im$(A64$x0, A64$x0, -4));
//	tb_trace_i("%x", A64$ldur_xt_$xn_im$(A64$x0, A64$x0, 4));
//	tb_trace_i("%x", A64$ldur_xt_$xn_im$(A64$x0, A64$sp, 8));

	// done test3
	test3(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

#if defined(TB_ARCH_ARM)
#ifdef TB_ARCH_ARM64
	__tb_asm__ __tb_volatile__("nop");

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
