#ifndef IT_PREFIX_H
#define IT_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "config.h"
#include "tbox/tbox.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// debug
#define IT_DEBUG 		itrace_CONFIG_DEBUG

// trace
#define it_print(fmt, arg ...)						do { asl_log(tb_null, tb_null, 4, "[itrace]: " fmt , ## arg); } while (0)
#if IT_DEBUG
# 	define it_trace(fmt, arg ...)					do { asl_log(tb_null, tb_null, 4, "[itrace]: " fmt , ## arg); } while (0)
#else
# 	define it_trace(fmt, arg ...)
#endif

// check
#define it_check_return(x)							do { if (!(x)) return ; } while (0)
#define it_check_return_val(x, v)					do { if (!(x)) return (v); } while (0)
#define it_check_goto(x, b)							do { if (!(x)) goto b; } while (0)
#define it_check_break(x)							{ if (!(x)) break ; }
#define it_check_continue(x)						{ if (!(x)) continue ; }

// assert
#if IT_DEBUG
# 	define it_assert(x)								do { if (!(x)) { it_print("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); } } while(0)
# 	define it_assert_return(x)						do { if (!(x)) { it_print("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); return ; } } while(0)
# 	define it_assert_return_val(x, v)				do { if (!(x)) { it_print("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); return (v); } } while(0)
# 	define it_assert_goto(x, b)						do { if (!(x)) { it_print("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); goto b; } } while(0)
# 	define it_assert_break(x)						{ if (!(x)) { it_print("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); break ; } }
# 	define it_assert_continue(x)					{ if (!(x)) { it_print("[assert]: expr: %s, func: %s, line: %d, file: %s:", #x, __FUNCTION__, __LINE__, __FILE__); continue ; } }
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

#endif


