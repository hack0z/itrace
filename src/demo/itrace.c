/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include <mach/mach.h>
#ifdef TB_CONFIG_OS_MAC
# 	include <mach/mach_vm.h>
#endif
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <sys/sysctl.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

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

// arm thread state
#ifndef ARM_THREAD_STATE
# 	define ARM_THREAD_STATE 		(1)
#endif

// arm64 thread state
#ifndef ARM_THREAD_STATE64 
# 	define ARM_THREAD_STATE64 		(1)
#endif

// x86 thread state
#ifndef x86_THREAD_STATE32
# 	define x86_THREAD_STATE32 		(1)
#endif

// x86_64 thread state
#ifndef x86_THREAD_STATE64
# 	define x86_THREAD_STATE64 		(4)
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */

#ifdef TB_CONFIG_OS_IOS
extern kern_return_t mach_vm_region
(
	vm_map_t target_task,
	mach_vm_address_t *address,
	mach_vm_size_t *size,
	vm_region_flavor_t flavor,
	vm_region_info_t info,
	mach_msg_type_number_t *infoCnt,
	mach_port_t *object_name
);

extern kern_return_t mach_vm_read_overwrite
(
	vm_map_t target_task,
	mach_vm_address_t address,
	mach_vm_size_t size,
	mach_vm_address_t data,
	mach_vm_size_t *outsize
);

extern kern_return_t mach_vm_allocate
(
	vm_map_t target,
	mach_vm_address_t *address,
	mach_vm_size_t size,
	int flags
);

extern kern_return_t mach_vm_protect
(
	vm_map_t target_task,
	mach_vm_address_t address,
	mach_vm_size_t size,
	boolean_t set_maximum,
	vm_prot_t new_protection
);

extern kern_return_t mach_vm_write
(
	vm_map_t target_task,
	mach_vm_address_t address,
	vm_offset_t data,
	mach_msg_type_number_t dataCnt
);
#endif

#ifdef TB_ARCH_ARM64
typedef bool (*it_MSHookProcess_t)(pid_t pid, tb_char_t const* library);
#endif

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
typedef struct __it_arm_thread_state_t
{
	tb_uint32_t 		r[13];
	tb_uint32_t 		sp;
	tb_uint32_t 		lr;
	tb_uint32_t 		pc;
	tb_uint32_t 		cpsr;

}it_arm_thread_state_t;

typedef struct __it_arm_thread_state64_t
{
	tb_uint64_t 		x[29];
	tb_uint64_t 		fp;
	tb_uint64_t 		lr;
	tb_uint64_t 		sp;
	tb_uint64_t 		pc;
	tb_uint32_t 		cpsr;

}it_arm_thread_state64_t;

typedef struct __it_x86_thread_state32_t
{
	tb_uint32_t 		eax, ebx, ecx, edx, edi, esi, ebp, esp, ss, eflags, eip, cs, ds, es, fs, gs;

}it_x86_thread_state32_t;

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
	// trace
//	tb_trace_d("sym: %s, %p", sym, value);

	// save
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
	tb_assert_and_check_return_val(symoff && vma, tb_false);

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
	return tb_true;
}
static kern_return_t it_stuff(task_t task, cpu_type_t* cputype, it_addr_bundle_t* addrs)
{
	// make the optimizer happy
	*cputype = 0; 

	// init the task info
	task_dyld_info_data_t 	info = {0};
	mach_msg_type_number_t	count = TASK_DYLD_INFO_COUNT;
	if (task_info(task, TASK_DYLD_INFO, (task_info_t) &info, &count)) return tb_false;

	// read all image info
	union 
	{
		it_dyld_all_image_infos_t 		data;
		it_dyld_all_image_infos_64_t 	data64;

	} u;
	mach_vm_size_t data_size = sizeof(u);
	if (info.all_image_info_size < data_size) data_size = info.all_image_info_size;
	if (mach_vm_read_overwrite(task, info.all_image_info_addr, data_size, it_address_cast(&u), &data_size)) return tb_false;
	if (u.data.version <= 1) return tb_false;

	// read mach header
#if defined(TB_ARCH_x86) || defined(TB_ARCH_x64) || defined(TB_ARCH_ARM64)
	tb_bool_t proc64 = u.data64.dyldImageLoadAddress > 0? tb_true : tb_false;
#else
	tb_bool_t proc64 = tb_false;
#endif 
	tb_trace_d("proc64: %p", proc64);	
	struct mach_header 	mach_hdr = {0};
	mach_vm_address_t 	dyldImageLoadAddress = proc64? u.data64.dyldImageLoadAddress : u.data.dyldImageLoadAddress;
	if (mach_vm_read_overwrite(task, dyldImageLoadAddress, (mach_vm_size_t)sizeof(mach_hdr), it_address_cast(&mach_hdr), &data_size)) return tb_false;

	// swap?
	tb_bool_t 				swap = (mach_hdr.magic == MH_CIGAM || mach_hdr.magic == MH_CIGAM_64)? tb_true : tb_false;
	tb_trace_d("swap: %u", swap);

	// save sputype
	*cputype = it_swap_u32(mach_hdr.cputype);

	// read cmds
	mach_vm_size_t 			sizeofcmds = it_swap_u32(mach_hdr.sizeofcmds);
	struct load_command* 	cmds = malloc(sizeofcmds);
	tb_bool_t 				mh64 = (mach_hdr.magic == MH_MAGIC_64 || mach_hdr.magic == MH_CIGAM_64)? tb_true : tb_false;
	tb_trace_d("mh64: %u", mh64);	
	if (mach_vm_read_overwrite(task, dyldImageLoadAddress + (mh64 ? sizeof(struct mach_header_64) : sizeof(struct mach_header)), (mach_vm_size_t)sizeofcmds, it_address_cast(cmds), &sizeofcmds)) return tb_false;

	// read symtab
	mach_vm_address_t 		slide;
	it_symtab_bundle_t 		symtab;
	tb_size_t 				nlist_size = mh64 ? sizeof(struct nlist_64) : sizeof(struct nlist);
	if (!it_find_symtab_addrs(dyldImageLoadAddress, mach_hdr.ncmds, sizeofcmds, cmds, swap, nlist_size, &symtab, &slide)) return tb_false;

	// read strs & syms
	tb_char_t* 		strs = malloc(symtab.strsize);
	tb_pointer_t 	syms = malloc(symtab.nsyms * nlist_size);
	if (mach_vm_read_overwrite(task, symtab.straddr, (mach_vm_size_t)(symtab.strsize), it_address_cast(strs), &data_size)) return tb_false;
	if (mach_vm_read_overwrite(task, symtab.symaddr, (mach_vm_size_t)(symtab.nsyms * nlist_size), it_address_cast(syms), &data_size)) return tb_false;

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
	return tb_true;
}
static tb_bool_t it_inject(pid_t pid, tb_char_t const* path) 
{
	// check
	tb_assert_and_check_return_val(pid && path, tb_false);

	// trace
	tb_trace_d("inject: pid: %lu, path: %s: ..", (tb_size_t)pid, path);

	// for arm64
#ifdef TB_ARCH_ARM64
	{
		// init library
		tb_bool_t 	ok = tb_false;
		tb_handle_t library = tb_dynamic_init("/usr/lib/libsubstrate.dylib");
		if (library)
		{
			// trace
			tb_trace_d("library: %p", library);

			// the func
			it_MSHookProcess_t pMSHookProcess = tb_dynamic_func(library, "MSHookProcess");
			if (pMSHookProcess)
			{
				// trace
				tb_trace_d("MSHookProcess: %p", pMSHookProcess);

				// hook process
				ok = pMSHookProcess(pid, path)? tb_true : tb_false;
			}

			// exit library
			tb_dynamic_exit(library);

			// trace
			tb_trace_i("%s", ok? "ok" : "no");

			// ok?
			return ok;
		}
	}
#endif

	// pid => task
	task_t task = 0;
	if (task_for_pid(mach_task_self(), (tb_int_t)pid, &task)) 
	{
		tb_trace_i("task_for_pid: %lu failed, errno: %d", (tb_size_t)pid, errno);
		return tb_false;
	}

	// trace
	tb_trace_d("task: %u", task);

	// stuff
	cpu_type_t cputype; it_addr_bundle_t addrs;
	if (!it_stuff(task, &cputype, &addrs)) return tb_false;

	// trace
	tb_trace_d("dlopen: %p", addrs.dlopen);
	tb_trace_d("syscall: %p", addrs.syscall);

	// alloc stack 
	mach_vm_address_t stack_address = 0;
	if (mach_vm_allocate(task, &stack_address, it_stack_size, VM_FLAGS_ANYWHERE)) return tb_false;

	// write path
	mach_vm_address_t stack_end = stack_address + it_stack_size - 0x100;
	if (mach_vm_write(task, stack_address, (vm_offset_t)it_address_cast(path), strlen(path) + 1)) return tb_false;

	/* the first one is the return address
	 *
	 * syscall(SYS_bsdthread_create, 0xdeadbeef, 0xdeadbeef, 128 * 1024, 0, 0)
	 */
	tb_uint32_t args_32[] = {0, 360, 0xdeadbeef, 0xdeadbeef, 128 * 1024, 0, 0};
	tb_uint64_t args_64[] = {0, 360, 0xdeadbeef, 0xdeadbeef, 128 * 1024, 0, 0};

	// init thread state 
	union
	{
		it_arm_thread_state_t 		arm;
		it_arm_thread_state64_t 	arm64;
		it_x86_thread_state32_t 	x86;
		it_x86_thread_state64_t 	x64;
		natural_t 					nat;

	}state;
	thread_state_flavor_t 			state_flavor;
	mach_msg_type_number_t 			state_count;
	memset(&state, 0, sizeof(state));	

	// init thread state for the cpu type
	switch (cputype)
	{
	case CPU_TYPE_ARM:
		{
			tb_trace_i("cputype: arm");
			memcpy(&state.arm.r[0], args_32 + 1, 4 * sizeof(tb_uint32_t));
			if (mach_vm_write(task, stack_end, (vm_offset_t)it_address_cast(args_32 + 5), 2 * sizeof(tb_uint32_t))) return tb_false;

			state.arm.sp 	= (tb_uint32_t) stack_end;
			state.arm.pc 	= (tb_uint32_t) addrs.syscall;
			state.arm.lr 	= (tb_uint32_t) args_32[0];

			state_flavor 	= ARM_THREAD_STATE;
			state_count 	= sizeof(state.arm) / sizeof(state.nat);

			// trace
			tb_trace_d("init: pc: %x", state.arm.pc);
			tb_trace_d("init: lr: %x", state.arm.lr);
			tb_trace_d("init: sp: %x", state.arm.sp);
		}
		break;
	case CPU_TYPE_ARM64:
		{
			tb_trace_i("cputype: arm64");
			memcpy(&state.arm64.x[0], args_64 + 1, 6 * sizeof(tb_uint64_t));

			state.arm64.sp 	= (tb_uint64_t) stack_end;
//			state.arm64.fp 	= (tb_uint64_t) stack_end;
			state.arm64.pc 	= (tb_uint64_t) addrs.syscall;
			state.arm64.lr 	= (tb_uint64_t) args_64[0];

			state_flavor 	= ARM_THREAD_STATE64;
			state_count 	= sizeof(state.arm64) / sizeof(state.nat);

			// trace
			tb_trace_d("init: pc: %llx", state.arm64.pc);
			tb_trace_d("init: lr: %llx", state.arm64.lr);
			tb_trace_d("init: sp: %llx", state.arm64.sp);
		}
		break;
	case CPU_TYPE_X86:
		{
			tb_trace_i("cputype: x86");
			if (mach_vm_write(task, stack_end, (vm_offset_t)it_address_cast(args_32), 7 * 4)) return tb_false;

			state.x86.esp 	= state.x86.ebp = (tb_uint32_t) stack_end;
			state.x86.eip 	= (tb_uint32_t)addrs.syscall;

			state_flavor 	= x86_THREAD_STATE32;
			state_count 	= sizeof(state.x86) / sizeof(state.nat);
		}
		break;
	case CPU_TYPE_X86_64:
		{
			tb_trace_i("cputype: x64");
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
		tb_trace_i("cputype: unknown: %lx", (tb_size_t)cputype);
		return tb_false;
	}

	// init a remote thread
	thread_act_t thread = 0;
	if (thread_create(task, &thread)) return tb_false;

	// trace
	tb_trace_d("init: thread: %x", thread);

	// alloc port
	mach_port_t exc = 0;
	mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &exc);
	if (mach_port_insert_right(mach_task_self(), exc, exc, MACH_MSG_TYPE_MAKE_SEND)) return tb_false;

	// swap port
	exception_mask_t 		em[2];
	exception_handler_t 	eh[2];
	exception_behavior_t 	eb[2];
	thread_state_flavor_t 	ef[2];
	mach_msg_type_number_t 	em_count = 2;
	if (task_swap_exception_ports(task, EXC_MASK_BAD_ACCESS, exc, EXCEPTION_STATE_IDENTITY, state_flavor, em, &em_count, eh, eb, ef)) return tb_false;
	tb_assert_and_check_return_val(em_count <= 1, tb_false);

	// resume thread, done: syscall(SYS_bsdthread_create, 0xdeadbeef, 0xdeadbeef, 128 * 1024, 0, 0)
	if (thread_set_state(thread, state_flavor, &state.nat, state_count)) return tb_false;
	if (thread_resume(thread)) return tb_false;

	// we expect three exceptions: one from thread when it returns, one from the new thread when it calls the fake handler, and one from the new thread when it returns from dlopen.
	tb_bool_t started_dlopen = tb_false;
	while (1) 
	{
		// recv msg
		it_exception_message_t msg;
		if (mach_msg_overwrite(tb_null, MACH_RCV_MSG, 0, sizeof(msg), exc, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL, (tb_pointer_t) &msg, sizeof(msg))) return tb_false;

		// trace
		tb_trace_d("recv: msg: from thread: %x", msg.thread.name);

		// check
		tb_assert_and_check_return_val((msg.Head.msgh_bits & MACH_MSGH_BITS_COMPLEX), tb_false);
		tb_assert_and_check_return_val((msg.msgh_body.msgh_descriptor_count != 0), tb_false);
		tb_assert_and_check_return_val((msg.Head.msgh_size >= offsetof(it_exception_message_t, old_state)), tb_false);
		tb_assert_and_check_return_val((msg.old_stateCnt == state_count), tb_false);
		tb_assert_and_check_return_val((msg.Head.msgh_size >= offsetof(it_exception_message_t, old_state) + msg.old_stateCnt * sizeof(natural_t)), tb_false);

		// the msg state
		memcpy(&state, msg.old_state, sizeof(state));

		// dump
//		tb_dump_data((tb_byte_t const*)&state, sizeof(state));

		// done
		if (msg.thread.name == thread)
		{
			tb_trace_d("terminate");
			if (thread_terminate(thread)) return tb_false;
		} 
		else
		{
			// init cond
			tb_bool_t cond = tb_false;
			switch(cputype)
			{
			case CPU_TYPE_ARM: 		
				{
					// trace
					tb_trace_d("recv: pc: %x", state.arm.pc);
					tb_trace_d("recv: lr: %x", state.arm.lr);
					tb_trace_d("recv: sp: %x", state.arm.sp);

					// cond
					cond = ((state.arm.pc & ~1) == 0xdeadbeee)? tb_true : tb_false;
				}
				break;
			case CPU_TYPE_ARM64:
				{
					// trace
					tb_trace_d("recv: pc: %llx", state.arm64.pc);
					tb_trace_d("recv: lr: %llx", state.arm64.lr);
					tb_trace_d("recv: sp: %llx", state.arm64.sp);

					// cond
					cond = ((state.arm64.pc & ~1) == 0xdeadbeee)? tb_true : tb_false;
				}
				break;
			case CPU_TYPE_X86:
				cond = (state.x86.eip == 0xdeadbeef)? tb_true : tb_false; 
				break;
			case CPU_TYPE_X86_64:
				cond = (state.x64.rip == 0xdeadbeef)? tb_true : tb_false;
				break;
			}

			tb_trace_d("cond: %d, started_dlopen: %d", cond, started_dlopen);
			if (!cond)
			{
				// let the normal crash mechanism handle it
				task_set_exception_ports(task, em[0], eh[0], eb[0], ef[0]);
				tb_assert_and_check_return_val(0, tb_false);
			}
			else if (started_dlopen)
			{
				tb_trace_d("terminate");
				if (thread_terminate(msg.thread.name)) return tb_false;
				break;
			}
			else 
			{
				// done: dlopen(path, RTLD_LAZY)
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
				case CPU_TYPE_ARM64:
					{
						state.arm64.x[0] = (tb_uint64_t) stack_address;
						state.arm64.x[1] = RTLD_LAZY;
						state.arm64.pc = (tb_uint64_t) addrs.dlopen;
						state.arm64.lr = 0xdeadbeef;
					}
					break;
				case CPU_TYPE_X86:
					{
						tb_uint32_t stack_stuff[3] = {0xdeadbeef, (tb_uint32_t)stack_address, RTLD_LAZY};
						if (mach_vm_write(task, (mach_vm_address_t)state.x86.esp, (vm_offset_t)it_address_cast(&stack_stuff), sizeof(stack_stuff))) return tb_false;
					}
					state.x86.eip = (tb_uint32_t) addrs.dlopen;
					break;
				case CPU_TYPE_X86_64:
					{
						tb_uint64_t stack_stuff = 0xdeadbeef;
						if (mach_vm_write(task, (mach_vm_address_t)state.x64.rsp, (vm_offset_t)it_address_cast(&stack_stuff), sizeof(stack_stuff))) return tb_false;
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

				if (thread_set_state(msg.thread.name, state_flavor, &state.nat, state_count)) return tb_false;
				if (mach_msg(&reply.Head, MACH_SEND_MSG, reply.Head.msgh_size, 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL)) return tb_false;
				started_dlopen = tb_true;
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
	tb_trace_i("ok");
	return tb_true;
}
static pid_t it_pid(tb_char_t const* name)
{
	// check
	tb_assert_and_check_return_val(name, 0);

	// is pid?
	tb_size_t pid = tb_atoi(name);
	if (pid) return pid;

	// init
	struct kinfo_proc* p = tb_null;
	struct kinfo_proc* q = tb_null;
	tb_int_t 	mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
	tb_size_t 	miblen = 4;
	tb_size_t 	size = 0;
	tb_long_t 	ok = sysctl(mib, miblen, tb_null, &size, tb_null, 0);

	// walk
	do 
	{
		// grow
		size += size / 10;
		q = tb_ralloc(p, size);

		// no memory?
		if (!q)
		{
			if (p) tb_free(p);
			return 0;
		}

		// list
		p = q;
		ok = sysctl(mib, miblen, p, &size, tb_null, 0);

	} while (ok == -1 && errno == ENOMEM);

	// ok?
	if (ok == 0)
	{
		if (!(size % sizeof(struct kinfo_proc)))
		{
			tb_size_t i = 0;
			tb_size_t n = size / sizeof(struct kinfo_proc);

			// try accurate name
			for (i = 0; i < n; i++)
			{
				if (p[i].kp_proc.p_comm && !tb_stricmp(p[i].kp_proc.p_comm, name))
				{
					tb_trace_i("name: %s, pid: %u", p[i].kp_proc.p_comm, p[i].kp_proc.p_pid);
					pid = p[i].kp_proc.p_pid;
					break;
				}
			}
			
			// try other name
			if (!pid)
			{
				for (i = 0; i < n; i++)
				{
					if (p[i].kp_proc.p_comm && !tb_strnicmp(p[i].kp_proc.p_comm, name, tb_strlen(name)))
					{
						tb_trace_i("name: %s, pid: %u", p[i].kp_proc.p_comm, p[i].kp_proc.p_pid);
						pid = p[i].kp_proc.p_pid;
						break;
					}
				}
			}
		}
	}

	// free
	if (p) tb_free(p);

	// ok
	return pid;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t const** argv)
{
	// the itrace.dylib path
	tb_char_t path[PATH_MAX] = {0};
	if (!realpath(argv[2]? argv[2] : "./itrace.dylib", path)) return -1;

	// trace
	tb_trace_d("path: %s", path);

	// wait pid
	pid_t pid = 0;
	while (!(pid = it_pid(argv[1]))) tb_msleep(500);

	// wait some time
	tb_msleep(1000);

	// inject
	it_inject(pid, path);

	// ok
	return 0;
}
