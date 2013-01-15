/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <stdio.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#ifdef TB_ARCH_ARM
#	include <CoreGraphics/CoreGraphics.h>
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * decl
 */

// pool
tb_pointer_t 	objc_autoreleasePoolPush();
tb_void_t 		objc_autoreleasePoolPop(tb_pointer_t pool);

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */

@interface Test : NSObject

- (CGRect)test:(CGRect)r with:(NSString*)s;

@end

@implementation Test

- (CGRect)test:(CGRect)r with:(NSString*)s
{
	NSLog(@"%@", s);
	return r;
}

@end

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
	tb_pointer_t pool = objc_autoreleasePoolPush();
	if (pool)
	{
		Test* t = [[Test alloc] init];
		getchar();
		if (t) [t test:CGRectMake(1.0f, 2.0f, 100.0f, 200.0f) with:@"hello"];
		getchar();
		objc_autoreleasePoolPop(pool);
	}
	return 0;
}
