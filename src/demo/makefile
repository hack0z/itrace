# sub-makefile
include $(PRO_DIR)/prefix.mak

# ############################################################################
# names
# #
NAMES 						= demo itrace

# demo
demo_TYPE 					= BIN
demo_C_FILES 				+= demo
demo_LIBS 					+= tbox $(TB_CONFIG_DEMO_LIBS)
demo_LIB_DIR 				+= ..
demo_INC_DIR 				+= ..
demo_CXFLAGS 				+= -DTB_PRINT_TAG=\"itrace\" 
demo_CXFLAGS-$(DEBUG) 		+= -DTB_TRACE_ENABLE -DTB_ASSERT_ENABLE
demo_SUFFIX_CMD1 			+= $(TB_CONFIG_DEMO_CMD1)

# itrace
itrace_TYPE 				= BIN
itrace_C_FILES 				+= itrace
itrace_LIBS 				+= tbox $(TB_CONFIG_DEMO_LIBS)
itrace_LIB_DIR 				+= ..
itrace_INC_DIR 				+= ..
itrace_CXFLAGS 				+= -DTB_PRINT_TAG=\"itrace\" 
itrace_CXFLAGS-$(DEBUG) 	+= -DTB_TRACE_ENABLE -DTB_ASSERT_ENABLE 
itrace_SUFFIX_CMD1 			+= $(TB_CONFIG_DEMO_CMD1)

# suffix
include $(PRO_DIR)/suffix.mak

