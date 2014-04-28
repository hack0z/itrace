/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "arm.h"
#include "x86.h"
#include "x64.h"
#include <sys/mman.h>
#include <mach/mach.h>
#include <stdarg.h>
#include <sys/types.h>
#include <pthread.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <Foundation.h>
#include <CoreFoundation.h>
#include <asl.h>
#include "etype.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the root
#define IT_ROOT 			"/tmp/"
#define IT_PATH_CFG 		IT_ROOT "itrace.xml"

// the argument
#if defined(TB_ARCH_ARM)
# 	define it_chook_method_trace_args_begn 			(2 * sizeof(tb_pointer_t))
# 	define it_chook_method_trace_args_skip(trace) 	\
 	do \
	{ \
		if (trace->args_size >= trace->args_begn && !trace->args_skip) \
		{ \
			trace->args_size += 44; \
			trace->args_skip = 1; \
			__tb_volatile__ tb_size_t r0 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r0); \
			__tb_volatile__ tb_size_t r1 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r1); \
			__tb_volatile__ tb_size_t r2 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r2); \
			__tb_volatile__ tb_size_t r3 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r3); \
			__tb_volatile__ tb_size_t r4 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r4); \
			__tb_volatile__ tb_size_t r5 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r5); \
			__tb_volatile__ tb_size_t r6 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r6); \
			__tb_volatile__ tb_size_t r7 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r7); \
			__tb_volatile__ tb_size_t r8 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r8); \
			__tb_volatile__ tb_size_t r9 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r9); \
			__tb_volatile__ tb_size_t lr = tb_va_arg(trace->args_list, tb_size_t); tb_used(lr); \
		} \
 \
	} while (0); 
#elif defined(TB_ARCH_x86)
# 	define it_chook_method_trace_args_begn 			(0)
# 	define it_chook_method_trace_args_skip(trace) 	\
 	do \
	{ \
		if (trace->args_size >= trace->args_begn && !trace->args_skip) \
		{ \
			trace->args_size += 48; \
			trace->args_skip = 1; \
			__tb_volatile__ tb_size_t edi = tb_va_arg(trace->args_list, tb_size_t); tb_used(edi); \
			__tb_volatile__ tb_size_t esi = tb_va_arg(trace->args_list, tb_size_t); tb_used(esi); \
			__tb_volatile__ tb_size_t ebp = tb_va_arg(trace->args_list, tb_size_t); tb_used(ebp); \
			__tb_volatile__ tb_size_t esp = tb_va_arg(trace->args_list, tb_size_t); tb_used(esp); \
			__tb_volatile__ tb_size_t ebx = tb_va_arg(trace->args_list, tb_size_t); tb_used(ebx); \
			__tb_volatile__ tb_size_t edx = tb_va_arg(trace->args_list, tb_size_t); tb_used(edx); \
			__tb_volatile__ tb_size_t ecx = tb_va_arg(trace->args_list, tb_size_t); tb_used(ecx); \
			__tb_volatile__ tb_size_t eax = tb_va_arg(trace->args_list, tb_size_t); tb_used(eax); \
			__tb_volatile__ tb_size_t flags = tb_va_arg(trace->args_list, tb_size_t); tb_used(flags); \
			__tb_volatile__ tb_size_t ret = tb_va_arg(trace->args_list, tb_size_t); tb_used(ret); \
			__tb_volatile__ tb_size_t sef = tb_va_arg(trace->args_list, tb_size_t); tb_used(sef); \
			__tb_volatile__ tb_size_t sel = tb_va_arg(trace->args_list, tb_size_t); tb_used(sel); \
		} \
 \
	} while (0); 
#elif defined(TB_ARCH_x64)		
# 	define it_chook_method_trace_args_begn 			(4 * sizeof(tb_pointer_t))
# 	define it_chook_method_trace_args_skip(trace) 	\
 	do \
	{ \
		if (trace->args_size >= trace->args_begn && !trace->args_skip) \
		{ \
			trace->args_size += 128; \
			trace->args_skip = 1; \
			__tb_volatile__ tb_size_t r15 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r15); \
			__tb_volatile__ tb_size_t r14 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r14); \
			__tb_volatile__ tb_size_t r13 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r13); \
			__tb_volatile__ tb_size_t r12 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r12); \
			__tb_volatile__ tb_size_t r11 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r11); \
			__tb_volatile__ tb_size_t r10 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r10); \
			__tb_volatile__ tb_size_t r9 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r9); \
			__tb_volatile__ tb_size_t r8 = tb_va_arg(trace->args_list, tb_size_t); tb_used(r8); \
			__tb_volatile__ tb_size_t rsi = tb_va_arg(trace->args_list, tb_size_t); tb_used(rsi); \
			__tb_volatile__ tb_size_t rdi = tb_va_arg(trace->args_list, tb_size_t); tb_used(rdi); \
			__tb_volatile__ tb_size_t rdx = tb_va_arg(trace->args_list, tb_size_t); tb_used(rdx); \
			__tb_volatile__ tb_size_t rcx = tb_va_arg(trace->args_list, tb_size_t); tb_used(rcx); \
			__tb_volatile__ tb_size_t rbx = tb_va_arg(trace->args_list, tb_size_t); tb_used(rbx); \
			__tb_volatile__ tb_size_t rax = tb_va_arg(trace->args_list, tb_size_t); tb_used(rax); \
			__tb_volatile__ tb_size_t flags = tb_va_arg(trace->args_list, tb_size_t); tb_used(flags); \
			__tb_volatile__ tb_size_t ret = tb_va_arg(trace->args_list, tb_size_t); tb_used(ret); \
		} \
 \
	} while (0); 
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the uint8 type
typedef struct __it_uint8_t
{
	tb_uint8_t 		i;

}__tb_packed__ it_uint8_t;

// the uint16 type
typedef struct __it_uint16_t
{
	tb_uint16_t 	i;

}__tb_packed__ it_uint16_t;

// the float type
typedef struct __it_float_t
{
	tb_float_t 		f;

}__tb_packed__ it_float_t;

// the double type
typedef struct __it_double_t
{
	tb_double_t 	d;

}__tb_packed__ it_double_t;

// the chook method trace type
typedef struct __it_chook_method_trace_t
{
	// the argument list
	tb_va_list_t 	args_list;

	// the argument size
	tb_size_t 		args_size;

	// the argument begn
	tb_size_t 		args_begn;

	// the argument skip
	tb_size_t 		args_skip;

	// the info 
	tb_char_t 		info[4096 + 1];
	tb_size_t 		size;
	tb_size_t 		maxn;

	// the method
	Method 			meth;

}it_chook_method_trace_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the config
static tb_xml_node_t* 		g_cfg = tb_null;

// the objc implementation 
static IMP 					g_NSObject_respondsToSelector 	= tb_null;
static IMP 					g_NSString_UTF8String 			= tb_null;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tb_inline__ tb_void_t tb_clear_cache(tb_pointer_t start, tb_pointer_t end)
{
	// for ios
// 	sys_icache_invalidate(start, (tb_char_t*)end - (tb_char_t*)start);

	// for gcc
// 	__clear_cache(start, end);
// 	__builtin___clear_cache(start, end);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_objc(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_pointer_t) == argb);

	// init
#if 0
	tb_pointer_t 		o = tb_va_arg(trace->args_list, tb_pointer_t);
	tb_pointer_t 		d = o && [o respondsToSelector:@selector(description)]? [o description] : tb_null;
	tb_char_t const* 	s = d? [d UTF8String] : tb_null;
#else
	tb_pointer_t 		o = tb_va_arg(trace->args_list, tb_pointer_t);
	tb_pointer_t 		c = o? object_getClass(o) : tb_null;
	IMP 				get_description = c? class_getMethodImplementation(c, @selector(description)) : tb_null;
	tb_pointer_t 		d = get_description && g_NSObject_respondsToSelector(o, @selector(respondsToSelector:), @selector(description))? get_description(o, @selector(description)) : tb_null;
	tb_char_t const* 	s = d? (tb_char_t const*)g_NSString_UTF8String(d, @selector(UTF8String)) : tb_null;
#endif

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %s", s);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_selector(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(SEL) == argb);

	// init
	SEL 				sel = tb_va_arg(trace->args_list, SEL);
	tb_char_t const*	sel_name = sel? sel_getName(sel) : tb_null;

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": @selector(%s)", sel_name);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_sint8(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_sint32_t) == argb);

	// init
	tb_long_t val = (tb_sint8_t)tb_va_arg(trace->args_list, tb_sint32_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %ld", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_uint8(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_uint32_t) == argb);

	// init
	tb_size_t val = (tb_uint8_t)tb_va_arg(trace->args_list, tb_uint32_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %lu", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_sint16(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_sint32_t) == argb);

	// init
	tb_long_t val = (tb_sint16_t)tb_va_arg(trace->args_list, tb_sint32_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %ld", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_uint16(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_uint32_t) == argb);

	// init
	tb_size_t val = (tb_uint16_t)tb_va_arg(trace->args_list, tb_uint32_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %lu", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_sint32(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_sint32_t) == argb);

	// init
	tb_long_t val = (tb_sint32_t)tb_va_arg(trace->args_list, tb_sint32_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %ld", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_uint32(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_uint32_t) == argb);

	// init
	tb_size_t val = (tb_uint32_t)tb_va_arg(trace->args_list, tb_uint32_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %lu", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_sint64(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_sint64_t) == argb);

	// init
	tb_sint64_t val = (tb_sint64_t)tb_va_arg(trace->args_list, tb_sint64_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %lld", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_uint64(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_uint64_t) == argb);

	// init
	tb_uint64_t val = (tb_uint64_t)tb_va_arg(trace->args_list, tb_uint64_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %llu", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_long(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_long_t) == argb);

	// init
	tb_long_t val = (tb_long_t)tb_va_arg(trace->args_list, tb_long_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %ld", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_size(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_size_t) == argb);

	// init
	tb_size_t val = (tb_size_t)tb_va_arg(trace->args_list, tb_size_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %lu", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_float(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(it_float_t) == argb);

	// init
	it_float_t val = (it_float_t)tb_va_arg(trace->args_list, it_float_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %f", val.f);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_double(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(it_double_t) == argb);

	// init
	it_double_t val = (it_double_t)tb_va_arg(trace->args_list, it_double_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %lf", val.d);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_data(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_byte_t*) == argb);

	// init
	tb_byte_t* 	b = (tb_byte_t*)tb_va_arg(trace->args_list, tb_byte_t*);

	// is c-string?
	tb_byte_t* 	p = b;
	tb_byte_t* 	e = b + 4096;
	while (p < e && *p && tb_isalpha(*p)) p++;

	// format
	if (trace->size < trace->maxn) 
	{
		if (p < e && !*p) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %s", b);
		else if (b) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %#x %#x %#x %#x ...", b[0], b[1], b[2], b[3]);
		else trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": null");
	}
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_pointer(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(sizeof(tb_pointer_t) == argb);

	// init
	tb_pointer_t val = (tb_pointer_t)tb_va_arg(trace->args_list, tb_pointer_t);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": %p", val);
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_void(it_chook_method_trace_t* trace, tb_size_t argb)
{	
	// check
	tb_assert_return(!argb);

	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": <void>");
}
static __tb_inline__ tb_void_t it_chook_method_trace_argument_skip(it_chook_method_trace_t* trace, tb_size_t argb, tb_char_t const* argt)
{
	// unit
	tb_size_t unit = argb & (sizeof(tb_uint32_t) - 1);

	// skip
	if (!unit)
	{
		tb_size_t skip = argb / sizeof(tb_uint32_t);
		while (skip--)
		{
			__tb_volatile__ tb_uint32_t val = tb_va_arg(trace->args_list, tb_uint32_t); tb_used(&val);
		}
	}
	else if (unit == 2)
	{
		tb_size_t skip = argb / sizeof(it_uint16_t);
		while (skip--)
		{
			__tb_volatile__ it_uint16_t val = tb_va_arg(trace->args_list, it_uint16_t); tb_used(&val);
		}
	}
	else 
	{
		tb_size_t skip = argb;
		while (skip--)
		{
			__tb_volatile__ it_uint8_t val = tb_va_arg(trace->args_list, it_uint8_t); tb_used(&val);
		}
	}
	
	// format
	if (trace->size < trace->maxn) trace->size += tb_snprintf(trace->info + trace->size, trace->maxn - trace->size, ": <%s>", argt);
}
static __tb_inline__ tb_void_t it_chook_method_trace_arguments(it_chook_method_trace_t* trace)
{	
	// init the method type encoding
	tb_char_t const* type = method_getTypeEncoding(trace->meth);
	tb_assert_and_check_return(type);
	tb_trace_d("type: %s", type);

	// walk arguments
	tb_size_t 			argi = 0;
	tb_size_t 			argb = 0;
	tb_size_t 			argn = it_etype_arguments_number(type);
	tb_char_t 			argt[512];
	for (argi = 2; argi < argn; argi++, trace->args_size += argb)
	{
		// the argument type
		it_etype_argument_type(type, argi, argt, 512);
	
		// the argument size
		argb = it_etype_argument_size(type, argi);

		// trace
		tb_trace_d("argt: %s, argb: %lu", argt, argb);

		// skip
		it_chook_method_trace_args_skip(trace);

		// trace argument
		tb_char_t const* p = argt; if (*p == 'r') p++;
		if (!strcmp(p, "@")) it_chook_method_trace_argument_objc(trace, argb);
		else if (!strcmp(p, ":")) it_chook_method_trace_argument_selector(trace, argb);
		else if (!strcmp(p, "f")) it_chook_method_trace_argument_float(trace, argb);
		else if (!strcmp(p, "d")) it_chook_method_trace_argument_double(trace, argb);
		else if (!strcmp(p, "q")) it_chook_method_trace_argument_sint64(trace, argb);
		else if (!strcmp(p, "Q")) it_chook_method_trace_argument_uint64(trace, argb);
		else if (!strcmp(p, "i")) it_chook_method_trace_argument_sint32(trace, argb);
		else if (!strcmp(p, "I")) it_chook_method_trace_argument_uint32(trace, argb);
		else if (!strcmp(p, "s")) it_chook_method_trace_argument_sint16(trace, argb);
		else if (!strcmp(p, "S")) it_chook_method_trace_argument_uint16(trace, argb);
		else if (!strcmp(p, "c")) it_chook_method_trace_argument_sint8(trace, argb);
		else if (!strcmp(p, "C")) it_chook_method_trace_argument_uint8(trace, argb);
		else if (!strcmp(p, "l")) it_chook_method_trace_argument_long(trace, argb);
		else if (!strcmp(p, "L")) it_chook_method_trace_argument_size(trace, argb);
		else if (!strcmp(p, "*")) it_chook_method_trace_argument_data(trace, argb);
		else if (!strcmp(p, "v")) it_chook_method_trace_argument_void(trace, argb);
		else if (*p == '^') it_chook_method_trace_argument_pointer(trace, argb);
		else it_chook_method_trace_argument_skip(trace, argb, argt);
	}
}
static __tb_inline__ tb_void_t it_chook_method_trace_skip_return(it_chook_method_trace_t* trace)
{
	// init the return type
	tb_char_t type[512 + 1] = {0};
	method_getReturnType(trace->meth, type, 512);
	tb_trace_d("return: %s", type);

	// struct?
	if (type[0] == '{')
	{
		// init return type size
		tb_size_t size = it_etype_size(type);
		tb_trace_d("size: %lu", size);

		// has argument: struct*?
#ifdef TB_ARCH_ARM
		if (size > sizeof(tb_uint16_t))
#else
		if (size > sizeof(tb_uint64_t))
#endif
		{
			// skip the argument first
			tb_trace_d("skip");
			__tb_volatile__ tb_pointer_t pstruct = (tb_pointer_t)tb_va_arg(trace->args_list, tb_pointer_t); tb_used(pstruct);
			trace->args_size += sizeof(tb_pointer_t);
		}
	}
}
static tb_void_t it_chook_method_trace(tb_xml_node_t const* node, Method method, ...)
{
	tb_assert_and_check_return(node && method);

	// the class name
	tb_char_t const* cname = tb_scoped_string_cstr(&node->name);
	tb_assert_and_check_return(cname);

	// tracing arguments?
	tb_bool_t args_list = tb_true;
	if (node->ahead
		&& !tb_scoped_string_cstricmp(&node->ahead->name, "args_list")
		&& !tb_scoped_string_cstricmp(&node->ahead->data, "0"))
	{
		args_list = tb_false;
	}

#if 0
	{
		// the method name
		tb_char_t const* mname = sel_getName(method_getName(method));
		tb_trace_i("[%lx]: [%s %s]", (tb_size_t)pthread_self(), cname, mname? mname : "");
		tb_msleep(10);
	}
#endif

	if (args_list)
	{
		// init trace 
		it_chook_method_trace_t trace;
		tb_memset(&trace, 0, sizeof(it_chook_method_trace_t));
		trace.maxn = 4096;
		trace.meth = method;
		trace.args_begn = it_chook_method_trace_args_begn;

		// init args_list
		tb_va_start(trace.args_list, method);

		// skip return
		it_chook_method_trace_skip_return(&trace);
	
		// trace arguments
		it_chook_method_trace_arguments(&trace);

		// exit args_list
		tb_va_end(trace.args_list);

		// trace
		tb_char_t const* mname = sel_getName(method_getName(method));
		tb_trace_i("[%lx]: [%s %s]%s", (tb_size_t)pthread_self(), cname, mname? mname : "", trace.info);
	}
	else
	{
		// the method name
		tb_char_t const* mname = sel_getName(method_getName(method));
		tb_trace_i("[%lx]: [%s %s]", (tb_size_t)pthread_self(), cname, mname? mname : "");
	}
}
static __tb_inline__ tb_size_t it_chook_method_size_for_class(tb_char_t const* class_name)
{
	tb_assert_and_check_return_val(class_name, 0);

	tb_uint_t 	method_n = 0;
	Method* 	method_s = class_copyMethodList(objc_getClass(class_name), &method_n);

	if (method_s) free(method_s);
	method_s = tb_null;

	return method_n;
}
static __tb_inline__ tb_pointer_t it_chook_method_done_for_class(tb_xml_node_t const* node, tb_pointer_t mmapfunc, tb_cpointer_t mmaptail)
{
	// check
	tb_assert_and_check_return_val(node && mmapfunc && mmaptail, mmapfunc);

	// the class name
	tb_char_t const* class_name = tb_scoped_string_cstr(&node->name);
	tb_assert_and_check_return_val(class_name, mmapfunc);

	// init method list
	tb_uint_t 	method_n = 0;
	Method* 	method_s = class_copyMethodList(objc_getClass(class_name), &method_n);
//	tb_trace_i("class: %s, method: %u", class_name, method_n);
	tb_assert_and_check_return_val(method_n && method_s, mmapfunc);

	// walk methods
	tb_size_t i = 0;
	for (i = 0; i < method_n; i++)
	{
		Method method = method_s[i];
		if (method)
		{
			// the selector
//			SEL sel = method_getName(method);

			// the imp
			IMP imp = method_getImplementation(method);

			// trace
//			tb_trace_i("[%s %s]: %p", class_name, sel_getName(sel), imp);

			// ok?
			if (imp)
			{
				// hook
#if defined(TB_ARCH_ARM)
				tb_uint32_t* p = (tb_uint32_t*)mmapfunc;
				tb_assert_and_check_break(p + 11 <= (tb_uint32_t*)mmaptail);
				*p++ = A$push$r0_r9_lr$;
				*p++ = A$movw_rd_im(A$r0, ((tb_uint32_t)node) & 0xffff);
				*p++ = A$movt_rd_im(A$r0, ((tb_uint32_t)node) >> 16);
				*p++ = A$movw_rd_im(A$r1, ((tb_uint32_t)method) & 0xffff);
				*p++ = A$movt_rd_im(A$r1, ((tb_uint32_t)method) >> 16);
				*p++ = A$movw_rd_im(A$r9, ((tb_uint32_t)&it_chook_method_trace) & 0xffff);
				*p++ = A$movt_rd_im(A$r9, ((tb_uint32_t)&it_chook_method_trace) >> 16);
				*p++ = A$blx(A$r9);
				*p++ = A$pop$r0_r9_lr$;
				*p++ = A$ldr_rd_$rn_im$(A$pc, A$pc, 4 - 8);
				*p++ = (tb_uint32_t)imp;
				method_setImplementation(method, (IMP)mmapfunc);
				mmapfunc = p;
#elif defined(TB_ARCH_x86)
				tb_byte_t* p = (tb_byte_t*)mmapfunc;
				tb_assert_and_check_break(p + 32 <= (tb_byte_t*)mmaptail);

				x86$pushf(p);
				x86$pusha(p);
				x86$push$im(p, (tb_uint32_t)method);
				x86$push$im(p, (tb_uint32_t)node);
				x86$mov$eax$im(p, (tb_uint32_t)&it_chook_method_trace);
				x86$call$eax(p);
				x86$pop$eax(p);
				x86$pop$eax(p);
				x86$popa(p);
				x86$popf(p);

				x86$push$im(p, (tb_uint32_t)imp);				
				x86$ret(p);

//				while (((tb_uint32_t)p & 0x3)) x86$nop(p);				
				x86$nop(p);
				x86$nop(p);
				x86$nop(p);

				tb_assert_and_check_break(!((tb_uint32_t)p & 0x3));
				method_setImplementation(method, (IMP)mmapfunc);
				mmapfunc = p;
#elif defined(TB_ARCH_x64)
				tb_byte_t* p = (tb_byte_t*)mmapfunc;
				tb_assert_and_check_break(p + 96 <= (tb_byte_t*)mmaptail);

				// 78-bytes
				x64$pushf(p);
				x64$push$rax(p);
				x64$push$rbx(p);
				x64$push$rcx(p);
				x64$push$rdx(p);
				x64$push$rdi(p);
				x64$push$rsi(p);
				x64$push$r8(p);
				x64$push$r9(p);
				x64$push$r10(p);
				x64$push$r11(p);
				x64$push$r12(p);
				x64$push$r13(p);
				x64$push$r14(p);
				x64$push$r15(p);
				x64$mov$rdi$im(p, (tb_uint64_t)node);
				x64$mov$rsi$im(p, (tb_uint64_t)method);
				x64$mov$rbx$im(p, (tb_uint64_t)&it_chook_method_trace);
				x64$call$rbx(p);
				x64$pop$r15(p);
				x64$pop$r14(p);
				x64$pop$r13(p);
				x64$pop$r12(p);
				x64$pop$r11(p);
				x64$pop$r10(p);
				x64$pop$r9(p);
				x64$pop$r8(p);
				x64$pop$rsi(p);
				x64$pop$rdi(p);
				x64$pop$rdx(p);
				x64$pop$rcx(p);
				x64$pop$rbx(p);
				x64$pop$rax(p);
				x64$popf(p);

				// 14-bytes
//				x64$sub$rsp$im(p, 0x8);
//				x64$mov$rsp$im(p, (tb_uint32_t)imp);
				x64$push$im(p, (tb_uint32_t)imp);
				x64$mov$rsp4$im(p, (tb_uint32_t)((tb_uint64_t)imp >> 32));
				x64$ret(p);

//				while (((tb_uint32_t)p & 0x7)) x64$nop(p);
				x64$nop(p);
				x64$nop(p);
				x64$nop(p);
				x64$nop(p);
//				x64$nop(p);
//				x64$nop(p);

				tb_assert_and_check_break(!((tb_uint32_t)p & 0x7));
				method_setImplementation(method, (IMP)mmapfunc);
				mmapfunc = p;	
#endif
			}
		}
	}

	// free it
	if (method_s) free(method_s);
	method_s = tb_null;

	// ok
	return mmapfunc;
}
static __tb_inline__ tb_size_t it_chook_method_size()
{
	// check
	tb_assert_and_check_return_val(g_cfg, 0);

	// walk
	tb_xml_node_t* 	root = tb_xml_node_goto(g_cfg, "/itrace/class");
	tb_xml_node_t* 	node = tb_xml_node_chead(root);
	tb_size_t 		size = 0;
	while (node)
	{
		if (node->type == TB_XML_NODE_TYPE_ELEMENT) 
			size += it_chook_method_size_for_class(tb_scoped_string_cstr(&node->name));
		node = node->next;
	}

	// size
	return size;
}

static __tb_inline__ tb_pointer_t it_chook_method_done(tb_pointer_t mmapfunc, tb_cpointer_t mmaptail)
{
	// check
	tb_assert_and_check_return_val(g_cfg, tb_null);

	// walk
	tb_xml_node_t* 	root = tb_xml_node_goto(g_cfg, "/itrace/class");
	tb_xml_node_t* 	node = tb_xml_node_chead(root);
	while (node)
	{
		if (node->type == TB_XML_NODE_TYPE_ELEMENT) 
			mmapfunc = it_chook_method_done_for_class(node, mmapfunc, mmaptail);
		node = node->next;
	}

	// ok
	return mmapfunc;
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * init
 */
static __tb_inline__ tb_bool_t it_cfg_init()
{
	// check
	tb_check_return_val(!g_cfg, tb_true);

	// the root path
	tb_char_t path[PATH_MAX] = {0};
	getcwd(path, PATH_MAX);
	tb_trace_i("init: root: %s", path);

	// the itrace.xml path
	if (!realpath("./itrace.xml", path)) tb_strcpy(path, IT_PATH_CFG);
	tb_trace_i("init: cfg: %s: ..", path);

	// init
	tb_basic_stream_t* stream = tb_basic_stream_init_from_url(path);
	tb_assert_and_check_return_val(stream, tb_false);

	// open
	if (!tb_basic_stream_open(stream)) return tb_false;

	// init reader
	tb_handle_t reader = tb_xml_reader_init(stream);
	if (reader)
	{
		// load
		g_cfg = tb_xml_reader_load(reader);
		tb_assert(g_cfg);

		// exit reader & writer 
		tb_xml_reader_exit(reader);
	}
	
	// exit stream
	tb_basic_stream_exit(stream);

	// check
	if (!tb_xml_node_goto(g_cfg, "/itrace/class") && !tb_xml_node_goto(g_cfg, "/itrace/function"))
	{
		tb_xml_node_exit(g_cfg);
		g_cfg = tb_null;
	}

	// trace
	tb_trace_i("init: cfg: %s", g_cfg? "ok" : "no");

	// ok
	return g_cfg? tb_true : tb_false;
}
static __tb_inline__ tb_bool_t it_objc_init()
{
	g_NSObject_respondsToSelector 	= class_getMethodImplementation(objc_getClass("NSObject"), @selector(respondsToSelector:));
	g_NSString_UTF8String 			= class_getMethodImplementation(objc_getClass("NSString"), @selector(UTF8String));
	tb_assert_and_check_return_val(g_NSObject_respondsToSelector && g_NSString_UTF8String, tb_false);
	return tb_true;
}
static __tb_inline__ tb_bool_t it_chook_init()
{
	// check
	tb_assert_and_check_return_val(g_cfg, tb_false);

	// the method size
	tb_size_t method_size = it_chook_method_size();
	tb_assert_and_check_return_val(method_size, tb_false);

	// trace
	tb_trace_i("chook: method_size: %lu", method_size);

	// init mmap base
#if defined(TB_ARCH_ARM)
	tb_size_t 		mmapmaxn = method_size * 11;
	tb_size_t 		mmapsize = mmapmaxn * sizeof(tb_uint32_t);
#elif defined(TB_ARCH_x86)
	tb_size_t 		mmapmaxn = method_size * 32;
	tb_size_t 		mmapsize = mmapmaxn;
#elif defined(TB_ARCH_x64)
	tb_size_t 		mmapmaxn = method_size * 96;
	tb_size_t 		mmapsize = mmapmaxn;
#endif

	tb_byte_t* 		mmapbase = (tb_byte_t*)mmap(tb_null, mmapsize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	tb_assert_and_check_return_val(mmapbase != MAP_FAILED && mmapbase, tb_false);

	// trace
	tb_trace_i("mmapmaxn: %lu, mmapbase: %p", mmapmaxn, mmapbase);

	// hook
	tb_pointer_t 	mmaptail = it_chook_method_done(mmapbase, mmapbase + mmapsize);

	// clear cache
	if (mmapbase != mmaptail) tb_clear_cache(mmapbase, mmaptail);

	// protect: rx
	tb_long_t ok = mprotect(mmapbase, mmapsize, PROT_READ | PROT_EXEC);
	tb_assert_and_check_return_val(!ok, tb_false);

	// ok
	return tb_true;
}
static tb_void_t __attribute__((constructor)) it_init()
{
	// trace
	tb_trace_i("init: ..");

	// init cfg
	if (!it_cfg_init()) return ;

	// init objc
	if (!it_objc_init()) return ;

	// init chook	
	if (!it_chook_init()) return ;
	
	// trace
	tb_trace_i("init: ok");
}
