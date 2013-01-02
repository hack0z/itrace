/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <stdio.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t const** argv)
{
#if defined(TB_ARCH_ARM)
	__tb_asm__ __tb_volatile__("nop");
	__tb_asm__ __tb_volatile__("push {r0-r9, lr}");
	__tb_asm__ __tb_volatile__("bx r1");
	__tb_asm__ __tb_volatile__("pop {r0-r9, lr}");

#elif defined(TB_ARCH_x86)

	__tb_asm__ __tb_volatile__("nop");
	__tb_asm__ __tb_volatile__("pushal");
	__tb_asm__ __tb_volatile__("popal");
	__tb_asm__ __tb_volatile__("push 0x12345678");
	__tb_asm__ __tb_volatile__("ret");

#elif defined(TB_ARCH_x64)
#endif
	return 0;
}
