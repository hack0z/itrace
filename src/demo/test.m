/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <stdio.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * type
 */
typedef struct __it_int8_t
{
	tb_uint8_t 			i0;
	tb_sint8_t 			i1;

}it_int8_t;

typedef struct __it_int16_t
{
	tb_uint16_t 		i0;
	tb_sint16_t 		i1;

}it_int16_t;

typedef struct __it_int32_t
{
	tb_uint32_t 		i0;
	tb_sint32_t 		i1;

}it_int32_t;

typedef struct __it_int64_t
{
	tb_uint64_t 		i0;
	tb_sint64_t 		i1;

}it_int64_t;

typedef struct __it_float_t
{
	tb_float_t 			f0;
	tb_float_t 			f1;

}it_float_t;

typedef struct __it_double_t
{
	tb_double_t 		d0;
	tb_double_t 		d1;

}it_double_t;

typedef struct __it_data8_t
{
	tb_sint8_t 			s8[2];
	tb_uint8_t 			u8[2];

}it_data8_t;

typedef struct __it_data16_t
{
	tb_sint16_t 		s16[2];
	tb_uint16_t 		u16[2];

}it_data16_t;

typedef struct __it_data32_t
{
	tb_sint32_t 		s32[2];
	tb_uint32_t 		u32[2];

}it_data32_t;

typedef struct __it_data64_t
{
	tb_sint64_t 		s64[2];
	tb_uint64_t 		u64[2];

}it_data64_t;

typedef struct __it_datafd_t
{
	tb_float_t 			f[2];
	tb_double_t 		d[2];

}it_datafd_t;

typedef struct __it_all_t
{
	it_int8_t 			i8;	
	it_int16_t 			i16;	
	it_int32_t 			i32;	
	it_int64_t 			i64;	
	it_float_t 			f;	
	it_double_t 		d;	

	it_int8_t* 			pi8;	
	it_int16_t* 		pi16;	
	it_int32_t* 		pi32;	
	it_int64_t* 		pi64;	
	it_float_t* 		pf;	
	it_double_t* 		pd;	
	tb_char_t const* 	s;
	it_data8_t 			d8;
	it_data16_t 		d16;
	it_data32_t 		d32;
	it_data64_t 		d64;
	it_datafd_t 		dfd;

}it_all_t;

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

- (tb_int8_t)value_int8:(tb_int8_t)i with:(NSString*)s;
- (tb_int16_t)value_int16:(tb_int16_t)i with:(NSString*)s;
- (tb_int32_t)value_int32:(tb_int32_t)i with:(NSString*)s;
- (tb_int64_t)value_int64:(tb_int64_t)i with:(NSString*)s;
- (tb_float_t)value_float:(tb_float_t)i with:(NSString*)s;
- (tb_double_t)value_double:(tb_double_t)i with:(NSString*)s;

- (tb_int8_t*)value_pint8:(tb_int8_t*)i with:(NSString*)s;
- (tb_int16_t*)value_pint16:(tb_int16_t*)i with:(NSString*)s;
- (tb_int32_t*)value_pint32:(tb_int32_t*)i with:(NSString*)s;
- (tb_int64_t*)value_pint64:(tb_int64_t*)i with:(NSString*)s;
- (tb_float_t*)value_pfloat:(tb_float_t*)i with:(NSString*)s;
- (tb_double_t*)value_pdouble:(tb_double_t*)i with:(NSString*)s;
- (tb_byte_t*)value_pbyte:(tb_byte_t*)i with:(NSString*)s;
- (tb_char_t const*)value_cstring:(tb_char_t const*)i with:(NSString*)s;

- (tb_int8_t**)value_ppint8:(tb_int8_t**)i with:(NSString*)s;
- (tb_int16_t**)value_ppint16:(tb_int16_t**)i with:(NSString*)s;
- (tb_int32_t**)value_ppint32:(tb_int32_t**)i with:(NSString*)s;
- (tb_int64_t**)value_ppint64:(tb_int64_t**)i with:(NSString*)s;
- (tb_float_t**)value_ppfloat:(tb_float_t**)i with:(NSString*)s;
- (tb_double_t**)value_ppdouble:(tb_double_t**)i with:(NSString*)s;
- (tb_char_t const**)value_pcstring:(tb_char_t const**)i with:(NSString*)s;

- (it_data8_t)value_data8:(it_data8_t)i with:(NSString*)s;
- (it_data16_t)value_data16:(it_data16_t)i with:(NSString*)s;
- (it_data32_t)value_data32:(it_data32_t)i with:(NSString*)s;
- (it_data64_t)value_data64:(it_data64_t)i with:(NSString*)s;
- (it_datafd_t)value_datafd:(it_datafd_t)i with:(NSString*)s;

- (it_int8_t)struct_int8:(it_int8_t)i with:(NSString*)s;
- (it_int16_t)struct_int16:(it_int16_t)i with:(NSString*)s;
- (it_int32_t)struct_int32:(it_int32_t)i with:(NSString*)s;
- (it_int64_t)struct_int64:(it_int64_t)i with:(NSString*)s;
- (it_float_t)struct_float:(it_float_t)i with:(NSString*)s;
- (it_double_t)struct_double:(it_double_t)i with:(NSString*)s;
- (it_all_t)struct_all:(it_all_t)i with:(NSString*)s;

@end

@implementation Test


- (tb_int8_t)value_int8:(tb_int8_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int16_t)value_int16:(tb_int16_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int32_t)value_int32:(tb_int32_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int64_t)value_int64:(tb_int64_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_float_t)value_float:(tb_float_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_double_t)value_double:(tb_double_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int8_t*)value_pint8:(tb_int8_t*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int16_t*)value_pint16:(tb_int16_t*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int32_t*)value_pint32:(tb_int32_t*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int64_t*)value_pint64:(tb_int64_t*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_float_t*)value_pfloat:(tb_float_t*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_double_t*)value_pdouble:(tb_double_t*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_byte_t*)value_pbyte:(tb_byte_t*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_char_t const*)value_cstring:(tb_char_t const*)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int8_t**)value_ppint8:(tb_int8_t**)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int16_t**)value_ppint16:(tb_int16_t**)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int32_t**)value_ppint32:(tb_int32_t**)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_int64_t**)value_ppint64:(tb_int64_t**)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_float_t**)value_ppfloat:(tb_float_t**)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_double_t**)value_ppdouble:(tb_double_t**)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (tb_char_t const**)value_pcstring:(tb_char_t const**)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_data8_t)value_data8:(it_data8_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_data16_t)value_data16:(it_data16_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_data32_t)value_data32:(it_data32_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_data64_t)value_data64:(it_data64_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_datafd_t)value_datafd:(it_datafd_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_int8_t)struct_int8:(it_int8_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_int16_t)struct_int16:(it_int16_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_int32_t)struct_int32:(it_int32_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_int64_t)struct_int64:(it_int64_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_float_t)struct_float:(it_float_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_double_t)struct_double:(it_double_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
}
- (it_all_t)struct_all:(it_all_t)i with:(NSString*)s
{
	NSLog(@"%@", s);
	return i;
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
		// init
		it_all_t all;
		all.i8.i0 = 100;
		all.i8.i1 = -20;
		all.i16.i0 = 100;
		all.i16.i1 = -200;
		all.i32.i0 = 100;
		all.i32.i1 = -200;
		all.i64.i0 = 100;
		all.i64.i1 = -200;
		all.f.f0 = 100;
		all.f.f1 = -200;
		all.d.d0 = 100;
		all.d.d1 = -200;
		all.pi8 = &all.i8;
		all.pi16 = &all.i16;
		all.pi32 = &all.i32;
		all.pi64 = &all.i64;
		all.pf = &all.f;
		all.pd = &all.d;
		all.s = "hello";
		all.d8.u8[0] = 100;
		all.d8.u8[1] = 200;
		all.d8.s8[0] = (tb_sint8_t)-100;
		all.d8.s8[1] = (tb_sint8_t)-200;
		all.d16.u16[0] = 100;
		all.d16.u16[1] = 200;
		all.d16.s16[0] = (tb_sint16_t)-100;
		all.d16.s16[1] = (tb_sint16_t)-200;
		all.d32.u32[0] = 100;
		all.d32.u32[1] = 200;
		all.d32.s32[0] = -100;
		all.d32.s32[1] = -200;
		all.d64.u64[0] = 100;
		all.d64.u64[1] = 200;
		all.d64.s64[0] = -100;
		all.d64.s64[1] = -200;
		all.dfd.f[0] = 100;
		all.dfd.f[1] = 200;
		all.dfd.d[0] = -100;
		all.dfd.d[1] = -200;


		Test* t = [[Test alloc] init];
		getchar();
		[t value_int8:all.i8.i1 with:@"hello"];
		[t value_int16:all.i16.i1 with:@"hello"];
		[t value_int32:all.i32.i1 with:@"hello"];
		[t value_int64:all.i64.i1 with:@"hello"];
		[t value_float:all.f.f0 with:@"hello"];
		[t value_double:all.d.d0 with:@"hello"];
		
		[t value_pint8:&all.i8.i1 with:@"hello"];
		[t value_pint16:&all.i16.i1 with:@"hello"];
		[t value_pint32:&all.i32.i1 with:@"hello"];
		[t value_pint64:&all.i64.i1 with:@"hello"];
		[t value_pfloat:&all.f.f0 with:@"hello"];
		[t value_pdouble:&all.d.d0 with:@"hello"];
		[t value_cstring:all.s with:@"hello"];		
	
		[t value_ppint8:tb_null with:@"hello"];
		[t value_ppint16:tb_null with:@"hello"];
		[t value_ppint32:tb_null with:@"hello"];
		[t value_ppint64:tb_null with:@"hello"];
		[t value_ppfloat:tb_null with:@"hello"];
		[t value_ppdouble:tb_null with:@"hello"];
		[t value_pcstring:&all.s with:@"hello"];		

		[t value_data8:all.d8 with:@"hello"];
		[t value_data16:all.d16 with:@"hello"];
		[t value_data32:all.d32 with:@"hello"];
		[t value_data64:all.d64 with:@"hello"];
		[t value_datafd:all.dfd with:@"hello"];

		[t struct_int8:all.i8 with:@"hello"];
		[t struct_int16:all.i16 with:@"hello"];
		[t struct_int32:all.i32 with:@"hello"];
		[t struct_int64:all.i64 with:@"hello"];
		[t struct_float:all.f with:@"hello"];
		[t struct_double:all.d with:@"hello"];
		[t struct_all:all with:@"hello"];
		getchar();
		objc_autoreleasePoolPop(pool);
	}
	return 0;
}
