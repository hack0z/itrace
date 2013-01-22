#ifndef IT_ETYPE_H
#define IT_ETYPE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

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
	tb_bool_t offset_is_negative = TB_FALSE;

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
				offset_is_negative = TB_TRUE;
				typedesc += 1;
			}
			else offset_is_negative = TB_FALSE;

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
				offset_is_negative = TB_TRUE;
				typedesc += 1;
			}
			else offset_is_negative = TB_FALSE;

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
	tb_char_t const* 	tb = TB_NULL;
	tb_char_t const* 	te = TB_NULL;
	it_etype_argument_info(type, index, &tb, &ob);
	it_etype_argument_info(type, index + 1, &te, &oe);
	if (!oe) oe = it_etype_arguments_size(type);
	return ob < oe? oe - ob : 0;
}



#endif


