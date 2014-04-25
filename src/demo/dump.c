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

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t const** argv)
{
#if defined(TB_ARCH_ARM)
	__tb_asm__ __tb_volatile__("nop");
//	__tb_asm__ __tb_volatile__("push {r0-r9, lr}");
//	__tb_asm__ __tb_volatile__("bx r1");
//	__tb_asm__ __tb_volatile__("pop {r0-r9, lr}");

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
