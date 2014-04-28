#ifndef IT_ETYPE_H
#define IT_ETYPE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the etype type
typedef struct __it_etype_t
{
	// the type size
	tb_size_t 			size;

	// the pointer number
	tb_size_t 			ptrn;

	// the array element number
	tb_size_t 			arrn;

	// in struct?
	tb_size_t 			stct;

}it_etype_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * inlines
 */
static __tb_inline__ tb_long_t it_etype_subtype_util(tb_char_t const* type, tb_char_t end)
{
	tb_long_t 			level = 0;
	tb_char_t const* 	head = type;

	while (*type)
	{
		if (!*type || (!level && (*type == end)))
			return (tb_long_t)(type - head);

		switch (*type)
		{
		case ']': case '}': case ')': level--; break;
		case '[': case '{': case '(': level += 1; break;
		}

		type += 1;
	}

	tb_assert(0);
	return 0;
}

static __tb_inline__ tb_char_t const* it_etype_skip_first_type(tb_char_t const* type)
{
	while (1)
	{
		switch (*type++)
		{
		case 'O':	/* bycopy */
		case 'n':	/* in */
		case 'o':	/* out */
		case 'N':	/* inout */
		case 'r':	/* const */
		case 'V':	/* oneway */
		case '^':	/* pointers */
			break;

		case '@':   /* objects */
			if (type[0] == '?') type++;  /* Blocks */
			return type;

			/* arrays */
		case '[':
			while ((*type >= '0') && (*type <= '9'))
				type += 1;
			return type + it_etype_subtype_util (type, ']') + 1;

			/* structures */
		case '{':
			return type + it_etype_subtype_util (type, '}') + 1;

			/* unions */
		case '(':
			return type + it_etype_subtype_util (type, ')') + 1;

			/* basic types */
		default:
			return type;
		}
	}
}

static __tb_inline__ tb_size_t it_etype_arguments_number(tb_char_t const* typedesc)
{
	tb_size_t nargs;

	// First, skip the return type
	typedesc = it_etype_skip_first_type (typedesc);

	// Next, skip stack size
	while ((*typedesc >= '0') && (*typedesc <= '9'))
		typedesc += 1;

	// Now, we have the arguments - count how many
	nargs = 0;
	while (*typedesc)
	{
		// Traverse argument type
		typedesc = it_etype_skip_first_type (typedesc);

		// Skip GNU runtime's register parameter hint
		if (*typedesc == '+') typedesc++;

		// Traverse (possibly negative) argument offset
		if (*typedesc == '-')
			typedesc += 1;
		while ((*typedesc >= '0') && (*typedesc <= '9'))
			typedesc += 1;

		// Made it past an argument
		nargs += 1;
	}

	return nargs;
}
static __tb_inline__ tb_size_t it_etype_arguments_size(tb_char_t const* typedesc)
{
	tb_size_t		stack_size;

	// Get our starting points
	stack_size = 0;

	// Skip the return type
	typedesc = it_etype_skip_first_type (typedesc);

	// Convert ASCII number string to integer
	while ((*typedesc >= '0') && (*typedesc <= '9'))
		stack_size = (stack_size * 10) + (*typedesc++ - '0');

	return stack_size;
}
static __tb_inline__ tb_size_t it_etype_argument_info(tb_char_t const* typedesc, tb_size_t arg, tb_char_t const** type, tb_long_t* offset)
{
	tb_size_t nargs = 0;
	tb_long_t self_offset = 0;
	tb_bool_t offset_is_negative = tb_false;

	// First, skip the return type
	typedesc = it_etype_skip_first_type (typedesc);

	// Next, skip stack size
	while ((*typedesc >= '0') && (*typedesc <= '9'))
		typedesc += 1;

	// Now, we have the arguments - position typedesc to the appropriate argument
	while (*typedesc && nargs != arg)
	{
		// Skip argument type
		typedesc = it_etype_skip_first_type (typedesc);

		if (nargs == 0)
		{
			// Skip GNU runtime's register parameter hint
			if (*typedesc == '+') typedesc++;

			// Skip negative sign in offset
			if (*typedesc == '-')
			{
				offset_is_negative = tb_true;
				typedesc += 1;
			}
			else offset_is_negative = tb_false;

			while ((*typedesc >= '0') && (*typedesc <= '9'))
				self_offset = self_offset * 10 + (*typedesc++ - '0');
			if (offset_is_negative) self_offset = -(self_offset);
		}
		else
		{
			// Skip GNU runtime's register parameter hint
			if (*typedesc == '+') typedesc++;

			// Skip (possibly negative) argument offset
			if (*typedesc == '-') typedesc += 1;
			while ((*typedesc >= '0') && (*typedesc <= '9')) typedesc += 1;
		}

		nargs += 1;
	}

	if (*typedesc)
	{
		tb_long_t arg_offset = 0;

		*type	 = typedesc;
		typedesc = it_etype_skip_first_type (typedesc);

		if (arg == 0)
		{
			*offset = 0;
		}

		else
		{
			// Skip GNU register parameter hint
			if (*typedesc == '+') typedesc++;

			// Pick up (possibly negative) argument offset
			if (*typedesc == '-')
			{
				offset_is_negative = tb_true;
				typedesc += 1;
			}
			else offset_is_negative = tb_false;

			while ((*typedesc >= '0') && (*typedesc <= '9')) arg_offset = arg_offset * 10 + (*typedesc++ - '0');
			if (offset_is_negative) arg_offset = - arg_offset;

			*offset = arg_offset - self_offset;
		}

	}
	else
	{
		*type	= 0;
		*offset	= 0;
	}

	return nargs;
}

static __tb_inline__ tb_void_t it_etype_argument_type(tb_char_t const* t, tb_size_t index, tb_char_t* dst, tb_size_t dst_len)
{
	tb_size_t len;
	tb_char_t const* end;
	tb_long_t offset;

	if (!dst) return;
	if (!t) 
	{
		tb_strncpy(dst, "", dst_len);
		return;
	}

	it_etype_argument_info(t, index, &t, &offset);

	if (!t) 
	{
		tb_strncpy(dst, "", dst_len);
		return;
	}

	end = it_etype_skip_first_type(t);
	len = end - t;
	tb_strncpy(dst, t, tb_min(len, dst_len));
	if (len < dst_len) tb_memset(dst+len, 0, dst_len - len);
}

static __tb_inline__ tb_long_t it_etype_argument_offset(tb_char_t const* type, tb_size_t index)
{
	tb_long_t offset = 0;
	it_etype_argument_info(type, index, &type, &offset);
	return offset;
}

static __tb_inline__ tb_size_t it_etype_argument_size(tb_char_t const* type, tb_size_t index)
{
	tb_long_t 			ob = 0;
	tb_long_t 			oe = 0;
	tb_char_t const* 	tb = tb_null;
	tb_char_t const* 	te = tb_null;
	it_etype_argument_info(type, index, &tb, &ob);
	it_etype_argument_info(type, index + 1, &te, &oe);
	if (!oe) oe = it_etype_arguments_size(type);
	return ob < oe? oe - ob : 0;
}

static __tb_inline__ tb_char_t const* it_etype_size_type(it_etype_t* etype, tb_char_t const* type);
static __tb_inline__ tb_char_t const* it_etype_size_struct(it_etype_t* etype, tb_char_t const* type)
{
	// skip '{'
	tb_char_t const* p = type + 1;

	// check
	tb_assert_and_check_return_val(etype->arrn == 1, p);

	// in struct
	etype->stct = 1;

	// walk
	while (*p)
	{
		switch (*p)
		{
		case '=':
			p = it_etype_size_type(etype, p + 1);
			break;
		case '}':
			etype->stct = 0;
			return p + 1;
		default:
			p++;
			break;
		}
	}

	// ok
	return p;
}
static __tb_inline__ tb_char_t const* it_etype_size_array(it_etype_t* etype, tb_char_t const* type)
{
	// skip '['
	tb_char_t const* p = type + 1;

	// check
	tb_assert_and_check_return_val(etype->arrn == 1, p);

	// size
	etype->arrn = tb_atoi(p);
//	it_trace("arrn: %lu", etype->arrn);

	// skip size
	while (*p && *p >= '0' && *p <= '9') p++;

	// walk
	while (*p)
	{
		// end?
		if (*p == ']')
		{
			etype->arrn = 1;
			return p + 1;
		}
		// type
		else p = it_etype_size_type(etype, p);
	}

	// ok
	return p;
}
static __tb_inline__ tb_char_t const* it_etype_size_type(it_etype_t* etype, tb_char_t const* type)
{
	tb_char_t const* p = type;
	while (*p)
	{
		switch (*p)
		{
		case '@':
			etype->size += etype->arrn * sizeof(tb_pointer_t); p++;
			break;
		case ':': 
			etype->size += etype->ptrn? etype->arrn * sizeof(tb_pointer_t) : etype->arrn * sizeof(SEL); p++;
			break;
		case 'f': 
			etype->size += etype->ptrn? etype->arrn * sizeof(tb_pointer_t) : etype->arrn * sizeof(tb_float_t); p++;	
			break;
		case 'd': 
			etype->size += etype->ptrn? etype->arrn * sizeof(tb_pointer_t) : etype->arrn * sizeof(tb_double_t); p++;	
			break;
		case 'q': 
		case 'Q': 
			etype->size += etype->ptrn? etype->arrn * sizeof(tb_pointer_t) : etype->arrn * sizeof(tb_uint64_t); p++;	
			break;
		case 'i': 
		case 'I': 
			if (etype->ptrn) etype->size += etype->arrn * sizeof(tb_pointer_t);
			else etype->size += etype->arrn * sizeof(tb_uint32_t); p++;	
			break;
		case 'l': 
		case 'L': 
			if (etype->ptrn) etype->size += etype->arrn * sizeof(tb_pointer_t);
			else etype->size += etype->arrn * sizeof(tb_uint32_t); p++;	
			break;
		case 's': 
		case 'S': 
			if (etype->ptrn) etype->size += etype->arrn * sizeof(tb_pointer_t);
			else if (etype->arrn > 1 || etype->stct) etype->size += etype->arrn * sizeof(tb_uint16_t);
			else etype->size += etype->arrn * sizeof(tb_uint32_t); p++;	
			break;
		case 'c': 
		case 'C': 
			if (etype->ptrn) etype->size += etype->arrn * sizeof(tb_pointer_t);
			else if (etype->arrn > 1 || etype->stct) etype->size += etype->arrn * sizeof(tb_uint8_t);
			else etype->size += etype->arrn * sizeof(tb_uint32_t); p++;	
			break;
		case 'v':
			etype->size += etype->ptrn? etype->arrn * sizeof(tb_pointer_t) : 0; p++;
			break;
		case '*':
			etype->size += etype->arrn * sizeof(tb_pointer_t); p++; 
			break;
		case '{':
			if (etype->ptrn)
			{
				etype->size += etype->arrn * sizeof(tb_pointer_t); 
				p++;
			}
			else p = it_etype_size_struct(etype, p); 
			break;
		case '[':
			if (etype->ptrn)
			{
				etype->size += etype->arrn * sizeof(tb_pointer_t); 
				p++;
			}
			else p = it_etype_size_array(etype, p); 
			break;
		case '}':
		case ']':
			return p;
		case '^':
			etype->ptrn++;
			p++;
			break;
		case 'r':
		default:
			p++;
			break;
		}
	}

	return p;
}
static __tb_inline__ tb_size_t it_etype_size(tb_char_t const* type)
{
	it_etype_t etype = {0}; etype.arrn = 1;
	it_etype_size_type(&etype, type);
	return etype.size;
}


#endif


