# sub-makefile
include $(PRO_DIR)/prefix.mak

# ############################################################################
# names
# #
NAMES 							= itrace

# itrace
itrace_TYPE 					= DLL
itrace_M_FILES					+= itrace
itrace_LIBS						+= tbox
itrace_MXFLAGS 					+= -DTB_PRINT_TAG=\"itrace\" 
itrace_MXFLAGS-$(DEBUG) 		+= -DTB_TRACE_ENABLE -DTB_ASSERT_ENABLE

# demo
SUB_PROS 						= demo

# suffix
include $(PRO_DIR)/suffix.mak

