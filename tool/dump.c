
int main(int a, char** b)
{
#if 1
	asm volatile (".byte 0x4b");
	asm volatile (".byte 0xf6");
	asm volatile (".byte 0x42");
	asm volatile (".byte 0x01");
	asm volatile (".byte 0x01");
	asm volatile (".byte 0x23");
	asm volatile (".byte 0xc0");
	asm volatile (".byte 0xf2");
#else
	asm volatile("push {r0-r9, lr}");
	asm volatile("bx r1");
	asm volatile("nop");
	asm volatile("pop {r0-r9, lr}");
#endif
	return 0;
}
