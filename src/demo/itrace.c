/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <mach/mach.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// address cast
#define it_address_cast(x) 			((mach_vm_address_t)(tb_pointer_t)(x))

// swap
#define it_swap_u32(x) 				(swap ? __builtin_bswap32(x) : (x))
#define it_swap_u64(x) 				(swap ? __builtin_bswap64(x) : (x))

// stack size
#define it_stack_size 				((mach_vm_size_t)(32 * 1024))

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the all image info type
typedef struct __it_dyld_all_image_infos_t
{
	tb_uint32_t 		version;
	tb_uint32_t 		infoArrayCount;
	tb_uint32_t 		infoArray;
	tb_uint32_t 		notification;
	tb_uint8_t 			processDetachedFromSharedRegion;
	tb_uint8_t 			libSystemInitialized;
	tb_uint8_t 			pad[2];
	tb_uint32_t 		dyldImageLoadAddress;

}it_dyld_all_image_infos_t;

// the all image info type for 64-bits
typedef struct __it_dyld_all_image_infos_64_t
{
	tb_uint32_t 		version;
	tb_uint32_t 		infoArrayCount;
	tb_uint64_t 		infoArray;
	tb_uint64_t 		notification;
	tb_uint8_t 			processDetachedFromSharedRegion;
	tb_uint8_t 			libSystemInitialized;
	tb_uint8_t 			pad[6];
	tb_uint64_t 		dyldImageLoadAddress;

}it_dyld_all_image_infos_64_t;

// the thread state type
#define ARM_THREAD_STATE 1
typedef struct __it_arm_thread_state_t
{
	tb_uint32_t 		r[13];
	tb_uint32_t 		sp;
	tb_uint32_t 		lr;
	tb_uint32_t 		pc;
	tb_uint32_t 		cpsr;
}it_arm_thread_state_t;

#define x86_THREAD_STATE32 1
typedef struct __it_x86_thread_state32_t
{
	tb_uint32_t 		eax, ebx, ecx, edx, edi, esi, ebp, esp, ss, eflags, eip, cs, ds, es, fs, gs;

}it_x86_thread_state32_t;

#define x86_THREAD_STATE64 4
typedef struct __it_x86_thread_state64_t
{
	tb_uint64_t 		rax, rbx, rcx, rdx, rdi, rsi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, rip, rflags, cs, fs, gs;

}it_x86_thread_state64_t;

// the execution message type
#pragma pack(4)
typedef struct __it_exception_message_t
{
	mach_msg_header_t 			Head;
	mach_msg_body_t 			msgh_body;
	mach_msg_port_descriptor_t 	thread;
	mach_msg_port_descriptor_t 	task;
	NDR_record_t 				NDR;
	exception_type_t 			exception;
	mach_msg_type_number_t 		codeCnt;
	integer_t 					code[2];
	tb_int_t 					flavor;
	mach_msg_type_number_t 		old_stateCnt;
	natural_t 					old_state[144];

}it_exception_message_t;

typedef struct __it_exception_reply_t
{
	mach_msg_header_t 			Head;
	NDR_record_t 				NDR;
	kern_return_t 				RetCode;
	tb_int_t 					flavor;
	mach_msg_type_number_t 		new_stateCnt;
	natural_t 					new_state[144];

}it_exception_reply_t;
#pragma pack()

// the address bundle type
typedef struct __it_addr_bundle_t
{
	mach_vm_address_t 	dlopen;
	mach_vm_address_t 	syscall;

}it_addr_bundle_t;

// the symtab bundle type
typedef struct __it_symtab_bundle_t
{
	mach_vm_address_t 	symaddr;
	tb_uint32_t 		nsyms;
	mach_vm_address_t 	straddr;
	tb_uint32_t 		strsize;

}it_symtab_bundle_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * helper
 */
static __tb_inline__ tb_void_t it_handle_sym(tb_char_t const* sym, tb_uint32_t size, mach_vm_address_t value, it_addr_bundle_t* bundle)
{
	switch (sym[1])
	{
	case 'd':
		if (!strncmp(sym, "_dlopen", size)) bundle->dlopen = value;
		break;
	case 's':
		if (!strncmp(sym, "_syscall", size)) bundle->syscall = value;
		break;
	}
}
static tb_bool_t it_find_symtab_addrs(mach_vm_address_t dyldImageLoadAddress, tb_uint32_t ncmds, mach_vm_size_t sizeofcmds, struct load_command* cmds, tb_bool_t swap, tb_size_t nlist_size, it_symtab_bundle_t* symtab, mach_vm_address_t* slide_)
{	
	// init offset
	tb_uint32_t 			symoff = 0;
	tb_uint32_t 			stroff = 0;

	// clear
	memset(symtab, 0, sizeof(*symtab));

	// walk
	mach_vm_address_t 		vma = 0;
	struct load_command* 	lc = cmds;
	tb_size_t 				ln = ncmds;
	while (ln--)
	{
		tb_uint32_t cmdsize = it_swap_u32(lc->cmdsize);
		tb_assert(sizeofcmds >= sizeof(struct load_command) && sizeofcmds >= cmdsize);
		sizeofcmds -= cmdsize;

		if (!vma && it_swap_u32(lc->cmd) == LC_SEGMENT) 
		{
			struct segment_command* sc = (tb_pointer_t) lc;
			tb_assert(cmdsize >= sizeof(*sc));
			vma = it_swap_u32(sc->vmaddr);
		}
		else if (!vma && it_swap_u32(lc->cmd) == LC_SEGMENT_64) 
		{
			struct segment_command_64* sc = (tb_pointer_t) lc;
			tb_assert(cmdsize >= sizeof(*sc));
			vma = it_swap_u64(sc->vmaddr);
		}
		else if (it_swap_u32(lc->cmd) == LC_SYMTAB)
		{
			struct symtab_command* sc = (tb_pointer_t) lc;
			tb_assert(cmdsize >= sizeof(*sc));

			symoff = it_swap_u32(sc->symoff);
			symtab->nsyms = it_swap_u32(sc->nsyms);

			stroff = it_swap_u32(sc->stroff);
			symtab->strsize = it_swap_u32(sc->strsize);
			tb_assert(symtab->strsize < 10000000 && symtab->nsyms < 10000000);
		}

		// next
		lc = (tb_pointer_t) ((tb_char_t*) lc + it_swap_u32(lc->cmdsize));
	}

	// check
	tb_assert_and_check_return_val(symoff && vma, TB_FALSE);

	// slide
	mach_vm_address_t slide = dyldImageLoadAddress - vma;
	*slide_ = slide;

	// walk
	lc = cmds;
	ln = ncmds;
	while (ln--)
	{
#define CATCH(SWAP, off, size, addr) tb_assert(SWAP(sc->fileoff) + SWAP(sc->filesize) >= SWAP(sc->fileoff)); if (SWAP(sc->fileoff) <= (off) && (SWAP(sc->fileoff) + SWAP(sc->filesize) - (off)) >= (size)) (addr) = SWAP(sc->vmaddr) + slide + (off) - SWAP(sc->fileoff);		
		if (it_swap_u32(lc->cmd) == LC_SEGMENT)
		{
			struct segment_command* sc = (tb_pointer_t) lc;
			if(!vma) vma = it_swap_u32(sc->vmaddr);
			CATCH(it_swap_u32, symoff, symtab->nsyms * nlist_size, symtab->symaddr);
			CATCH(it_swap_u32, stroff, symtab->strsize, symtab->straddr);
		}
		else if (it_swap_u32(lc->cmd) == LC_SEGMENT_64)
		{
			struct segment_command_64* sc = (tb_pointer_t) lc;
			CATCH(it_swap_u64, symoff, symtab->nsyms * nlist_size, symtab->symaddr);
			CATCH(it_swap_u64, stroff, symtab->strsize, symtab->straddr);
		}
		lc = (tb_pointer_t) ((tb_char_t*)lc + it_swap_u32(lc->cmdsize));
	}

	tb_assert(symtab->straddr);
	tb_assert(symtab->symaddr);

	// ok
	return TB_TRUE;
}
static kern_return_t it_stuff(task_t task, cpu_type_t* cputype, it_addr_bundle_t* addrs)
{
	// make the optimizer happy
	*cputype = 0; 

	// init the task info
	task_dyld_info_data_t 	info = {0};
	mach_msg_type_number_t	count = TASK_DYLD_INFO_COUNT;
	if (task_info(task, TASK_DYLD_INFO, (task_info_t) &info, &count)) return TB_FALSE;

	// read all image info
	union 
	{
		it_dyld_all_image_infos_t 		data;
		it_dyld_all_image_infos_64_t 	data64;

	} u;
	mach_vm_size_t data_size = sizeof(u);
	if (info.all_image_info_size < data_size) data_size = info.all_image_info_size;
	if (mach_vm_read_overwrite(task, info.all_image_info_addr, data_size, it_address_cast(&u), &data_size)) return TB_FALSE;
	if (u.data.version <= 1) return TB_FALSE;

	// read mach header
#if defined(TB_ARCH_x86) || defined(TB_ARCH_x64)
	tb_bool_t proc64 = info.all_image_info_addr > 0? TB_TRUE : TB_FALSE;
#else
	tb_bool_t proc64 = TB_FALSE;
#endif 
	tb_trace("proc64: %p", proc64);	
	struct mach_header 	mach_hdr = {0};
	mach_vm_address_t 	dyldImageLoadAddress = proc64? u.data64.dyldImageLoadAddress : u.data.dyldImageLoadAddress;
	if (mach_vm_read_overwrite(task, dyldImageLoadAddress, (mach_vm_size_t)sizeof(mach_hdr), it_address_cast(&mach_hdr), &data_size)) return TB_FALSE;

	// swap?
	tb_bool_t 				swap = (mach_hdr.magic == MH_CIGAM || mach_hdr.magic == MH_CIGAM_64)? TB_TRUE : TB_FALSE;
	tb_trace("swap: %u", swap);

	// save sputype
	*cputype = it_swap_u32(mach_hdr.cputype);

	// read cmds
	mach_vm_size_t 			sizeofcmds = it_swap_u32(mach_hdr.sizeofcmds);
	struct load_command* 	cmds = malloc(sizeofcmds);
	tb_bool_t 				mh64 = (mach_hdr.magic == MH_MAGIC_64 || mach_hdr.magic == MH_CIGAM_64)? TB_TRUE : TB_FALSE;
	tb_trace("mh64: %u", mh64);	
	if (mach_vm_read_overwrite(task, dyldImageLoadAddress + (mh64 ? sizeof(struct mach_header_64) : sizeof(struct mach_header)), (mach_vm_size_t)sizeofcmds, it_address_cast(cmds), &sizeofcmds)) return TB_FALSE;

	// read symtab
	mach_vm_address_t 		slide;
	it_symtab_bundle_t 		symtab;
	tb_size_t 				nlist_size = mh64 ? sizeof(struct nlist_64) : sizeof(struct nlist);
	if (!it_find_symtab_addrs(dyldImageLoadAddress, mach_hdr.ncmds, sizeofcmds, cmds, swap, nlist_size, &symtab, &slide)) return TB_FALSE;

	// read strs & syms
	tb_char_t* 		strs = malloc(symtab.strsize);
	tb_pointer_t 	syms = malloc(symtab.nsyms * nlist_size);
	if (mach_vm_read_overwrite(task, symtab.straddr, (mach_vm_size_t)(symtab.strsize), it_address_cast(strs), &data_size)) return TB_FALSE;
	if (mach_vm_read_overwrite(task, symtab.symaddr, (mach_vm_size_t)(symtab.nsyms * nlist_size), it_address_cast(syms), &data_size)) return TB_FALSE;

	// read address
	memset(addrs, 0, sizeof(*addrs));
	if (mh64) 
	{
		struct nlist_64 const* nl = syms;
		while (symtab.nsyms--) 
		{
			tb_uint32_t strx = (tb_uint32_t) it_swap_u32(nl->n_un.n_strx);
			tb_assert(strx < symtab.strsize);
			it_handle_sym(strs + strx, symtab.strsize - strx, (mach_vm_address_t) it_swap_u64(nl->n_value) + slide, addrs);
			nl++;
		}
	} 
	else 
	{
		struct nlist const* nl = syms;
		while (symtab.nsyms--)
		{
			tb_uint32_t strx = it_swap_u32(nl->n_un.n_strx);
			tb_assert(strx < symtab.strsize);
			it_handle_sym(strs + strx, symtab.strsize - strx, (mach_vm_address_t) it_swap_u32(nl->n_value) + slide, addrs);
			nl++;
		}
	}
	tb_assert(addrs->dlopen);
	tb_assert(addrs->syscall);

	// free
	if (cmds) free(cmds);
	if (strs) free(strs);
	if (syms) free(syms);

	// ok
	return TB_TRUE;
}
static tb_bool_t it_inject(pid_t pid, tb_char_t const* path) 
{
	// check
	tb_assert_and_check_return_val(pid && path, TB_FALSE);

	// the real path
	tb_char_t rpath[PATH_MAX] = {0};
	if (!realpath(path, rpath)) return TB_FALSE;
	tb_trace("rpath: %s", rpath);

	// pid => task
	task_t task = 0;
	if (task_for_pid(mach_task_self(), (tb_int_t)pid, &task)) return TB_FALSE;
	tb_trace("pid: %u, task: %u", pid, task);

	// stuff
	cpu_type_t cputype; it_addr_bundle_t addrs;
	if (!it_stuff(task, &cputype, &addrs)) return TB_FALSE;
	tb_trace("cputype: %x, dlopen: %p", cputype, addrs.dlopen);

	// alloc stack 
	mach_vm_address_t stack_address = 0;
	if (mach_vm_allocate(task, &stack_address, it_stack_size, VM_FLAGS_ANYWHERE)) return TB_FALSE;

	// write path
	mach_vm_address_t stack_end = stack_address + it_stack_size - 0x100;
	if (mach_vm_write(task, stack_address, it_address_cast(rpath), strlen(rpath) + 1)) return TB_FALSE;

	// the first one is the return address
	tb_uint32_t args_32[] = {0, 360, 0xdeadbeef, 0xdeadbeef, 128 * 1024, 0, 0};
	tb_uint64_t args_64[] = {0, 360, 0xdeadbeef, 0xdeadbeef, 128 * 1024, 0, 0};

	// init state
	union
	{
		it_arm_thread_state_t 		arm;
		it_x86_thread_state32_t 	x86;
		it_x86_thread_state64_t 	x64;
		natural_t 					nat;

	}state;
	thread_state_flavor_t 			state_flavor;
	mach_msg_type_number_t 			state_count;
	memset(&state, 0, sizeof(state));	
	switch (cputype)
	{
	case CPU_TYPE_ARM:
		{
			(void) args_64;
			memcpy(&state.arm.r[0], args_32 + 1, 4 * 4);
			if (mach_vm_write(task, stack_end, it_address_cast(args_32 + 5), 2 * 4)) return TB_FALSE;

			state.arm.sp 	= (tb_uint32_t) stack_end;
			state.arm.pc 	= (tb_uint32_t) addrs.syscall;
			state.arm.lr 	= (tb_uint32_t) args_32[0];

			state_flavor 	= ARM_THREAD_STATE;
			state_count 	= sizeof(state.arm) / sizeof(state.nat);
		}
		break;
	case CPU_TYPE_X86:
		{
			if (mach_vm_write(task, stack_end, it_address_cast(args_32), 7 * 4)) return TB_FALSE;

			state.x86.esp 	= state.x86.ebp = (tb_uint32_t) stack_end;
			state.x86.eip 	= (tb_uint32_t) addrs.syscall;

			state_flavor 	= x86_THREAD_STATE32;
			state_count 	= sizeof(state.x86) / sizeof(state.nat);
		}
		break;
	case CPU_TYPE_X86_64:
		{
			state.x64.rdi 	= args_64[1];
			state.x64.rsi 	= args_64[2];
			state.x64.rdx 	= args_64[3];
			state.x64.rcx 	= args_64[4];
			state.x64.r8 	= args_64[5];
			state.x64.r9 	= args_64[6];

			state.x64.rsp 	= state.x64.rbp = stack_end;
			state.x64.rip 	= addrs.syscall;

			state_flavor 	= x86_THREAD_STATE64;
			state_count 	= sizeof(state.x64) / sizeof(state.nat);
		}
		break;
	default:
		return TB_FALSE;
	}

	// init a remote thread
	thread_act_t thread = 0;
	if (thread_create(task, &thread)) return TB_FALSE;

	// alloc port
	mach_port_t exc = 0;
	mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &exc);
	if (mach_port_insert_right(mach_task_self(), exc, exc, MACH_MSG_TYPE_MAKE_SEND)) return TB_FALSE;

	// swap port
	exception_mask_t 		em[2];
	exception_handler_t 	eh[2];
	exception_behavior_t 	eb[2];
	thread_state_flavor_t 	ef[2];
	mach_msg_type_number_t 	em_count = 2;
	if (task_swap_exception_ports(task, EXC_MASK_BAD_ACCESS, exc, EXCEPTION_STATE_IDENTITY, state_flavor, em, &em_count, eh, eb, ef)) return TB_FALSE;
	tb_assert_and_check_return_val(em_count <= 1, TB_FALSE);

	// resume thread
	if (thread_set_state(thread, state_flavor, &state.nat, state_count)) return TB_FALSE;
	if (thread_resume(thread)) return TB_FALSE;

	// we expect three exceptions: one from thread when it returns, one from the new thread when it calls the fake handler, and one from the new thread when it returns from dlopen.
	tb_bool_t started_dlopen = TB_FALSE;
	while (1) 
	{
		// wait msg
		it_exception_message_t msg;
		if (mach_msg_overwrite(TB_NULL, MACH_RCV_MSG, 0, sizeof(msg), exc, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL, (tb_pointer_t) &msg, sizeof(msg))) return TB_FALSE;

		// trace
		tb_trace("msg");

		// check
		tb_assert_and_check_return_val((msg.Head.msgh_bits & MACH_MSGH_BITS_COMPLEX)
			&& (msg.msgh_body.msgh_descriptor_count != 0)
			&& (msg.Head.msgh_size >= offsetof(it_exception_message_t, old_state))
			&& (msg.old_stateCnt == state_count)
			&& (msg.Head.msgh_size >= offsetof(it_exception_message_t, old_state) + msg.old_stateCnt * sizeof(natural_t)), TB_FALSE);
		memcpy(&state, msg.old_state, sizeof(state));

		if (msg.thread.name == thread)
		{
			if (thread_terminate(thread)) return TB_FALSE;
		} 
		else
		{
			// init cond
			tb_bool_t cond = TB_FALSE;
			switch(cputype)
			{
			case CPU_TYPE_ARM: 		cond = ((state.arm.pc & ~1) == 0xdeadbeee)? TB_TRUE : TB_FALSE; break;
			case CPU_TYPE_X86: 		cond = (state.x86.eip == 0xdeadbeef)? TB_TRUE : TB_FALSE; break;
			case CPU_TYPE_X86_64: 	cond = (state.x64.rip == 0xdeadbeef)? TB_TRUE : TB_FALSE; break;
			}

			if (!cond)
			{
				// let the normal crash mechanism handle it
				task_set_exception_ports(task, em[0], eh[0], eb[0], ef[0]);
				tb_assert_and_check_return_val(0, TB_FALSE);
			}
			else if (started_dlopen)
			{
				if (thread_terminate(msg.thread.name)) return TB_FALSE;
				break;
			}
			else 
			{
				switch(cputype)
				{
				case CPU_TYPE_ARM:
					{
						state.arm.r[0] = (tb_uint32_t) stack_address;
						state.arm.r[1] = RTLD_LAZY;
						state.arm.pc = (tb_uint32_t) addrs.dlopen;
						state.arm.lr = 0xdeadbeef;
					}
					break;
				case CPU_TYPE_X86:
					{
						tb_uint32_t stack_stuff[3] = {0xdeadbeef, (tb_uint32_t)stack_address, RTLD_LAZY};
						if (mach_vm_write(task, state.x86.esp, it_address_cast(&stack_stuff), sizeof(stack_stuff))) return TB_FALSE;
					}
					state.x86.eip = (tb_uint32_t) addrs.dlopen;
					break;
				case CPU_TYPE_X86_64:
					{
						tb_uint64_t stack_stuff = 0xdeadbeef;
						if (mach_vm_write(task, state.x64.rsp, it_address_cast(&stack_stuff), sizeof(stack_stuff))) return TB_FALSE;
						state.x64.rip = addrs.dlopen;
						state.x64.rdi = stack_address;
						state.x64.rsi = RTLD_LAZY;
					}
					break;
				}

				it_exception_reply_t reply;
				memcpy(&reply.Head, &msg.Head, sizeof(mach_msg_header_t));
				reply.Head.msgh_bits &= ~MACH_MSGH_BITS_COMPLEX;
				reply.Head.msgh_size = offsetof(it_exception_reply_t, new_state) + state_count * sizeof(natural_t);
				reply.Head.msgh_id += 100;
				memcpy(&reply.NDR, &msg.NDR, sizeof(NDR_record_t));
				reply.RetCode = 0;
				reply.flavor = state_flavor;
				reply.new_stateCnt = state_count;
				memcpy(&reply.new_state, &state, sizeof(state));

				if (thread_set_state(msg.thread.name, state_flavor, &state.nat, state_count)) return TB_FALSE;
				if (mach_msg(&reply.Head, MACH_SEND_MSG, reply.Head.msgh_size, 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL)) return TB_FALSE;
				started_dlopen = TB_TRUE;
			}
		}
	}

	// exit
	if (stack_address) vm_deallocate(task, stack_address, it_stack_size);
	if (thread)
	{
		thread_terminate(thread);
		mach_port_deallocate(mach_task_self(), thread);
	}
	if (task) mach_port_deallocate(mach_task_self(), task);
	if (exc) mach_port_deallocate(mach_task_self(), exc);

	// ok
	tb_trace("ok");
	return TB_TRUE;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t const** argv)
{
	// inject
	it_inject(tb_atoi(argv[1]), argv[2]);

	// ok
	return 0;
}
