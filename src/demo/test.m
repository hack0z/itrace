/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <stdio.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <Foundation/Foundation.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
	id 	g_NSMutableString 				= objc_getClass("NSMutableString");
	IMP g_NSMutableString_alloc 		= class_getMethodImplementation(g_NSMutableString, @selector(alloc));
	IMP g_NSMutableString_init 			= class_getMethodImplementation(g_NSMutableString, @selector(init));
	IMP g_NSMutableString_appendFormat 	= class_getMethodImplementation(g_NSMutableString, @selector(appendFormat:));
	IMP g_NSMutableString_UTF8String 	= class_getMethodImplementation(g_NSMutableString, @selector(UTF8String));
	IMP g_NSMutableString_release 		= class_getMethodImplementation(g_NSMutableString, @selector(release));

	tb_print("g_NSMutableString: %p", g_NSMutableString);
	tb_print("g_NSMutableString_alloc: %p", g_NSMutableString_alloc);
	tb_print("g_NSMutableString_init: %p", g_NSMutableString_init);
	tb_print("g_NSMutableString_appendFormat: %p", g_NSMutableString_appendFormat);
	tb_print("g_NSMutableString_UTF8String: %p", g_NSMutableString_UTF8String);
	tb_print("g_NSMutableString_release: %p", g_NSMutableString_release);

	getchar();
	tb_pointer_t pool = objc_autoreleasePoolPush();
	if (pool)
	{
//		id trace = g_NSMutableString_alloc(g_NSMutableString, @selector(alloc));
//		id trace = [[NSMutableString alloc] init];
		id trace = [NSMutableString alloc];
		tb_print("trace: %p", trace);
		trace = [trace init];
//		trace = g_NSMutableString_init(trace, @selector(init));
		tb_print("trace: %p", trace);

		g_NSMutableString_appendFormat(trace, @selector(appendFormat:), @"[%s %s]", "NSMutableString", "hello world!");
		tb_print("trace: %s", g_NSMutableString_UTF8String(trace, @selector(UTF8String)));
			
		g_NSMutableString_release(trace, @selector(release));

		objc_autoreleasePoolPop(pool);
	}

	return 0;
}
