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
#include "asl.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// trace
#define it_trace(fmt, arg ...)						do { asl_log(TB_NULL, TB_NULL, 4, "[itrace]: " fmt , ## arg); } while (0)

// check
#define it_check_return(x)							do { if (!(x)) return ; } while (0)
#define it_check_return_val(x, v)					do { if (!(x)) return (v); } while (0)
#define it_check_goto(x, b)							do { if (!(x)) goto b; } while (0)
#define it_check_break(x)							{ if (!(x)) break ; }
#define it_check_continue(x)						{ if (!(x)) continue ; }

// assert
#if IT_DEBUG
# 	define it_assert(x)								do { if (!(x)) { it_trace("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); } } while(0)
# 	define it_assert_return(x)						do { if (!(x)) { it_trace("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); return ; } } while(0)
# 	define it_assert_return_val(x, v)				do { if (!(x)) { it_trace("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); return (v); } } while(0)
# 	define it_assert_goto(x, b)						do { if (!(x)) { it_trace("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); goto b; } } while(0)
# 	define it_assert_break(x)						{ if (!(x)) { it_trace("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); break ; } }
# 	define it_assert_continue(x)					{ if (!(x)) { it_trace("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); continue ; } }
# 	define it_assert_and_check_return(x)			it_assert_return(x)
# 	define it_assert_and_check_return_val(x, v)		it_assert_return_val(x, v)
# 	define it_assert_and_check_goto(x, b)			it_assert_goto(x, b)
# 	define it_assert_and_check_break(x)				it_assert_break(x)
# 	define it_assert_and_check_continue(x)			it_assert_continue(x)
#else
# 	define it_assert(x)
# 	define it_assert_return(x)
# 	define it_assert_return_val(x, v)
# 	define it_assert_goto(x, b)
# 	define it_assert_break(x)
# 	define it_assert_continue(x)
# 	define it_assert_and_check_return(x)			it_check_return(x)
# 	define it_assert_and_check_return_val(x, v)		it_check_return_val(x, v)
# 	define it_assert_and_check_goto(x, b)			it_check_goto(x, b)
# 	define it_assert_and_check_break(x)				it_check_break(x)
# 	define it_assert_and_check_continue(x)			it_check_continue(x)
#endif

// the root
#define IT_ROOT 			"/tmp/"
#define IT_PATH_CFG 		IT_ROOT "itrace.xml"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */
static tb_xml_node_t* 		g_cfg = TB_NULL;
static id 					g_NSMutableString = TB_NULL;
static IMP 					g_NSMutableString_alloc = TB_NULL;
static IMP 					g_NSMutableString_init = TB_NULL;
static IMP 					g_NSMutableString_appendFormat = TB_NULL;
static IMP 					g_NSMutableString_UTF8String = TB_NULL;
static IMP 					g_NSMutableString_release = TB_NULL;

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */

// clear cache for gcc
tb_void_t __clear_cache(tb_char_t* beg, tb_char_t* end);

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
#ifdef TB_ARCH_ARM
static tb_void_t it_chook_method_puts(tb_xml_node_t const* node, Method method, ...)
{
	it_assert_and_check_return(node && method);

	// the class name
	tb_char_t const* cname = tb_pstring_cstr(&node->name);
	it_assert_and_check_return(cname);

	// tracing arguments?
	tb_bool_t args = TB_TRUE;
	if (node->ahead
		&& !tb_pstring_cstricmp(&node->ahead->name, "args")
		&& !tb_pstring_cstricmp(&node->ahead->data, "0"))
	{
		args = TB_FALSE;
	}

	if (args)
	{
		// init pool
		@autoreleasepool
		{
			// the method name
			tb_char_t const* mname = sel_getName(method_getName(method));

			// init trace
			NSMutableString* trace = [[NSMutableString alloc] init];
			//NSMutableString* trace = g_NSMutableString_init(g_NSMutableString_alloc(g_NSMutableString, @selector(alloc)), @selector(init));
			it_assert_and_check_return(trace);
			g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @"[%s %s]", cname, mname? mname : "");

			// init args
			tb_va_list_t vl;
			tb_va_start(vl, method);

			// the arguments
			// self, sel, args ...
			tb_size_t argi = 0;
			tb_size_t argn = method_getNumberOfArguments(method);
			tb_char_t type[512 + 1];
			for (argi = 2; argi < argn; argi++)
			{
				memset(type, 0, 513);
				method_getArgumentType(method, argi, type, 512);
				if (argi == 4)
				{
					__tb_volatile__ tb_size_t r0 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r1 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r2 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r3 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r4 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r5 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r6 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r7 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r8 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t r9 = tb_va_arg(vl, tb_size_t);
					__tb_volatile__ tb_size_t lr = tb_va_arg(vl, tb_size_t);
				}
				if (!strcmp(type, "@"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %@", tb_va_arg(vl, tb_pointer_t));
				}
				else if (!strcasecmp(type, "f"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %f", tb_va_arg(vl, tb_float_t));	
				}
				else if (!strcasecmp(type, "i"))
				{				
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %ld", tb_va_arg(vl, tb_long_t));	
				}
				else if (!strcasecmp(type, "c"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %lu", tb_va_arg(vl, tb_size_t));
				}
				else if (!strcasecmp(type, "r*"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %s", tb_va_arg(vl, tb_char_t const*));
				}
				else 
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": <type(%s)>", type);
					__tb_volatile__ tb_pointer_t p = tb_va_arg(vl, tb_size_t);
				}
			}
			
			// exit args
			tb_va_end(vl);

			// trace
			it_trace("[%x]: %s", (tb_size_t)pthread_self(), g_NSMutableString_UTF8String(trace, @selector(UTF8String)));

			// exit
			g_NSMutableString_release(trace, @selector(release));
		}
	}
	else
	{
		// the method name
		tb_char_t const* mname = sel_getName(method_getName(method));
		it_trace("[%x]: [%s %s]", (tb_size_t)pthread_self(), cname, mname? mname : "");
	}
}
#elif defined(TB_ARCH_x86)
static tb_void_t it_chook_method_puts(tb_xml_node_t const* node, Method method, ...)
{
	it_assert_and_check_return(node && method);

	// the class name
	tb_char_t const* cname = tb_pstring_cstr(&node->name);
	it_assert_and_check_return(cname);

	// tracing arguments?
	tb_bool_t args = TB_TRUE;
	if (node->ahead
		&& !tb_pstring_cstricmp(&node->ahead->name, "args")
		&& !tb_pstring_cstricmp(&node->ahead->data, "0"))
	{
		args = TB_FALSE;
	}

	if (args)
	{
		// init pool
		tb_pointer_t pool = objc_autoreleasePoolPush();
		if (pool)
		{
			// the method name
			tb_char_t const* mname = sel_getName(method_getName(method));

			// init trace
			NSMutableString* trace = [[NSMutableString alloc] init];
			//NSMutableString* trace = g_NSMutableString_init(g_NSMutableString_alloc(g_NSMutableString, @selector(alloc)), @selector(init));
			it_assert_and_check_return(trace);
			g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @"[%s %s]", cname, mname? mname : "");

			// init args
			tb_va_list_t vl;
			tb_va_start(vl, method);
			__tb_volatile__ tb_size_t eax = tb_va_arg(vl, tb_size_t);
			__tb_volatile__ tb_size_t ret = tb_va_arg(vl, tb_size_t);
			__tb_volatile__ tb_size_t sef = tb_va_arg(vl, tb_size_t);
			__tb_volatile__ tb_size_t sel = tb_va_arg(vl, tb_size_t);
		
			// the arguments
			tb_size_t argi = 0;
			tb_size_t argn = method_getNumberOfArguments(method);
			tb_char_t type[512 + 1];
			for (argi = 2; argi < argn; argi++)
			{
				memset(type, 0, 513);
				method_getArgumentType(method, argi, type, 512);
				//it_trace("type: %s", type);
				if (!strcmp(type, "@"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %@", tb_va_arg(vl, tb_pointer_t));
				}
				else if (!strcasecmp(type, "f"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %f", tb_va_arg(vl, tb_float_t));	
				}
				else if (!strcasecmp(type, "i"))
				{				
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %ld", tb_va_arg(vl, tb_long_t));	
				}
				else if (!strcasecmp(type, "c"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %lu", tb_va_arg(vl, tb_size_t));
				}
				else if (!strcasecmp(type, "r*"))
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": %s", tb_va_arg(vl, tb_char_t const*));
				}
				else 
				{
					g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @": <type(%s)>", type);
					__tb_volatile__ tb_size_t p = tb_va_arg(vl, tb_size_t);
				}
			}
			
			// exit args
			tb_va_end(vl);

			// trace
			it_trace("[%x]: %s", (tb_size_t)pthread_self(), g_NSMutableString_UTF8String(trace, @selector(UTF8String)));

			// exit
			g_NSMutableString_release(trace, @selector(release));

			// exit pool
			objc_autoreleasePoolPop(pool);
		}
	}
	else
	{
		// the method name
		tb_char_t const* mname = sel_getName(method_getName(method));
		it_trace("[%x]: [%s %s]", (tb_size_t)pthread_self(), cname, mname? mname : "");
	}
}
#elif defined(TB_ARCH_x64)
static tb_void_t it_chook_method_puts(tb_xml_node_t const* node, Method method, ...)
{
	it_trace("sssssssssss");
}
#endif

static __tb_inline__ tb_size_t it_chook_method_size_for_class(tb_char_t const* class_name)
{
	it_assert_and_check_return_val(class_name, 0);

	tb_size_t 	method_n = 0;
	Method* 	method_s = class_copyMethodList(objc_getClass(class_name), &method_n);

	if (method_s) free(method_s);
	method_s = TB_NULL;

	return method_n;
}
static __tb_inline__ tb_pointer_t it_chook_method_done_for_class(tb_xml_node_t const* node, tb_pointer_t mmapfunc, tb_cpointer_t mmaptail)
{
	// check
	it_assert_and_check_return_val(node && mmapfunc && mmaptail, mmapfunc);

	// the class name
	tb_char_t const* class_name = tb_pstring_cstr(&node->name);
	it_assert_and_check_return_val(class_name, mmapfunc);

	// init method list
	tb_size_t 	method_n = 0;
	Method* 	method_s = class_copyMethodList(objc_getClass(class_name), &method_n);
//	it_trace("class: %s, method: %u", class_name, method_n);
	it_assert_and_check_return_val(method_n && method_s, mmapfunc);

	// walk methods
	tb_size_t i = 0;
	for (i = 0; i < method_n; i++)
	{
		Method method = method_s[i];
		if (method)
		{
			// the selector
			SEL sel = method_getName(method);

			// the imp
			IMP imp = method_getImplementation(method);

			// trace
//			it_trace("[%s %s]: %p", class_name, sel_getName(sel), imp);

			// ok?
			if (imp)
			{
				// hook
#if defined(TB_ARCH_ARM)
				tb_uint32_t* p = (tb_uint32_t*)mmapfunc;
				it_assert_and_check_break(p + 11 <= mmaptail);
				*p++ = A$push$r0_r9_lr$;
				*p++ = A$movw_rd_im(A$r0, ((tb_uint32_t)node) & 0xffff);
				*p++ = A$movt_rd_im(A$r0, ((tb_uint32_t)node) >> 16);
				*p++ = A$movw_rd_im(A$r1, ((tb_uint32_t)method) & 0xffff);
				*p++ = A$movt_rd_im(A$r1, ((tb_uint32_t)method) >> 16);
				*p++ = A$movw_rd_im(A$r9, ((tb_uint32_t)&it_chook_method_puts) & 0xffff);
				*p++ = A$movt_rd_im(A$r9, ((tb_uint32_t)&it_chook_method_puts) >> 16);
				*p++ = A$blx(A$r9);
				*p++ = A$pop$r0_r9_lr$;
				*p++ = A$ldr_rd_$rn_im$(A$pc, A$pc, 4 - 8);
				*p++ = (tb_uint32_t)imp;
				method_setImplementation(method, (IMP)mmapfunc);
				mmapfunc = p;
#elif defined(TB_ARCH_x86)
				tb_byte_t* p = (tb_byte_t*)mmapfunc;
				it_assert_and_check_break(p + 28 <= mmaptail);

				x86$push$eax(p);
				x86$push$im(p, (tb_uint32_t)method);
				x86$push$im(p, (tb_uint32_t)node);
				x86$mov$eax$im(p, (tb_uint32_t)&it_chook_method_puts);
				x86$call$eax(p);
				x86$add$esp$im(p, 0x8);
				x86$pop$eax(p);

				x86$push$im(p, (tb_uint32_t)imp);				
				x86$ret(p);

				it_assert_and_check_break(!((tb_uint32_t)p & 0x3));
				method_setImplementation(method, (IMP)mmapfunc);
				mmapfunc = p;
#elif defined(TB_ARCH_x64)
				tb_byte_t* p = (tb_byte_t*)mmapfunc;
				it_assert_and_check_break(p + 28 <= mmaptail);

				x64$push$rax(p);
				x64$push$im(p, (tb_uint64_t)method);
				x64$push$im(p, (tb_uint64_t)node);
				x64$mov$rax$im(p, (tb_uint64_t)&it_chook_method_puts);
				x64$call$rax(p);
				x64$add$rsp$im(p, 0x8);
				x64$pop$rax(p);

				x64$push$im(p, (tb_uint64_t)imp);				
				x64$ret(p);

				it_assert_and_check_break(!((tb_uint64_t)p & 0x3));
				method_setImplementation(method, (IMP)mmapfunc);
				mmapfunc = p;	
#endif
			}
		}
	}

	// free it
	if (method_s) free(method_s);
	method_s = TB_NULL;

	// ok
	return mmapfunc;
}
static __tb_inline__ tb_size_t it_chook_method_size()
{
	// check
	it_assert_and_check_return_val(g_cfg, 0);

	// walk
	tb_xml_node_t* 	root = tb_xml_node_goto(g_cfg, "/itrace/class");
	tb_xml_node_t* 	node = tb_xml_node_chead(root);
	tb_size_t 		size = 0;
	while (node)
	{
		if (node->type == TB_XML_NODE_TYPE_ELEMENT) 
			size += it_chook_method_size_for_class(tb_pstring_cstr(&node->name));
		node = node->next;
	}

	// size
	return size;
}

static __tb_inline__ tb_pointer_t it_chook_method_done(tb_pointer_t mmapfunc, tb_cpointer_t mmaptail)
{
	// check
	it_assert_and_check_return_val(g_cfg, TB_NULL);

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
	it_check_return_val(!g_cfg, TB_TRUE);

	// trace
	it_trace("init: cfg: ..");

	// init
	tb_gstream_t* gst = tb_gstream_init_from_url(IT_PATH_CFG);
	it_assert_and_check_return_val(gst, TB_FALSE);

	// open
	if (!tb_gstream_bopen(gst)) return TB_FALSE;

	// init reader
	tb_handle_t reader = tb_xml_reader_init(gst);
	if (reader)
	{
		// load
		g_cfg = tb_xml_reader_load(reader);
		it_assert(g_cfg);

		// exit reader & writer 
		tb_xml_reader_exit(reader);
	}
	
	// exit stream
	tb_gstream_exit(gst);

	// check
	if (!tb_xml_node_goto(g_cfg, "/itrace/class") && !tb_xml_node_goto(g_cfg, "/itrace/function"))
	{
		tb_xml_node_exit(g_cfg);
		g_cfg = TB_NULL;
	}

	// trace
	it_trace("init: cfg: %s", g_cfg? "ok" : "no");

	// ok
	return g_cfg? TB_TRUE : TB_FALSE;
}
static __tb_inline__ tb_bool_t it_objc_init()
{
	// trace
	it_trace("init: objc: ..");

	// implementation
	g_NSMutableString 				= objc_getClass("NSMutableString");
	g_NSMutableString_alloc 		= class_getMethodImplementation(g_NSMutableString, @selector(alloc));
	g_NSMutableString_init 			= class_getMethodImplementation(g_NSMutableString, @selector(init));
	g_NSMutableString_appendFormat 	= class_getMethodImplementation(g_NSMutableString, @selector(appendFormat:));
	g_NSMutableString_UTF8String 	= class_getMethodImplementation(g_NSMutableString, @selector(UTF8String));
	g_NSMutableString_release 		= class_getMethodImplementation(g_NSMutableString, @selector(release));

	// check
	it_assert_and_check_return_val(g_NSMutableString_alloc, TB_FALSE);
	it_assert_and_check_return_val(g_NSMutableString_init, TB_FALSE);
	it_assert_and_check_return_val(g_NSMutableString_appendFormat, TB_FALSE);
	it_assert_and_check_return_val(g_NSMutableString_UTF8String, TB_FALSE);
	it_assert_and_check_return_val(g_NSMutableString_release, TB_FALSE);

	// trace
	it_trace("init: objc: ok");

	// ok
	return TB_TRUE;
}
static __tb_inline__ tb_bool_t it_chook_init()
{
	// check
	it_assert_and_check_return_val(g_cfg, TB_FALSE);

	// the method size
	tb_size_t method_size = it_chook_method_size();
	it_assert_and_check_return_val(method_size, TB_FALSE);
	it_trace("chook: method_size: %u", method_size);

	// init mmap base
#if defined(TB_ARCH_ARM)
	tb_size_t 		mmapmaxn = method_size * 11;
	tb_size_t 		mmapsize = mmapmaxn * sizeof(tb_uint32_t);
#elif defined(TB_ARCH_x86)
	tb_size_t 		mmapmaxn = method_size * 28;
	tb_size_t 		mmapsize = mmapmaxn;
#elif defined(TB_ARCH_x64)
	tb_size_t 		mmapmaxn = method_size * 28;
	tb_size_t 		mmapsize = mmapmaxn;
#endif

	tb_byte_t* 		mmapbase = (tb_byte_t*)mmap(TB_NULL, mmapsize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	it_assert_and_check_return_val(mmapbase != MAP_FAILED && mmapbase, TB_FALSE);
	it_trace("mmapmaxn: %u, mmapbase: %p", mmapmaxn, mmapbase);

	// hook
	tb_pointer_t 	mmaptail = it_chook_method_done(mmapbase, mmapbase + mmapmaxn);

	// clear cache
	if (mmapbase != mmaptail) __clear_cache(mmapbase, mmaptail);

	// protect: rx
	tb_long_t ok = mprotect(mmapbase, mmapsize, PROT_READ | PROT_EXEC);
	it_assert_and_check_return_val(!ok, TB_FALSE);

	// ok
	return TB_TRUE;
}
static tb_void_t __attribute__((constructor)) it_init()
{
	// trace
	it_trace("init: ..");

	// init objc
	if (!it_objc_init()) return ;

	// init cfg
	if (!it_cfg_init()) return ;

	// init chook	
	if (!it_chook_init()) return ;
	
	// trace
	it_trace("init: ok");
}
