# sub-makefile
include $(PRO_DIR)/prefix.mak

# ############################################################################
# names
# #
NAMES 						= demo itrace test dump

# root
ifeq ($(PLAT),ios)
SDKROOT 					= /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDK).sdk/
endif

ifeq ($(PLAT),mac)
SDKROOT 					= /Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$(SDK).sdk/
endif

FWKROOT 					= $(SDKROOT)/System/Library/Frameworks

# demo
demo_TYPE 					= BIN
demo_C_FILES 				+= demo
demo_LIBS 					+= tbox $(TB_CONFIG_DEMO_LIBS)
demo_LIB_DIR 				+= ..
demo_INC_DIR 				+= ..
demo_CXFLAGS 				+= -DTB_PRINT_TAG=\"itrace\" 
demo_CXFLAGS-$(DEBUG) 		+= -DTB_TRACE_ENABLE -DTB_ASSERT_ENABLE
demo_SUFFIX_CMD1 			+= $(TB_CONFIG_DEMO_CMD1)

# dump
dump_TYPE 					= BIN
dump_C_FILES 				+= dump
dump_LIBS 					+= tbox $(TB_CONFIG_DEMO_LIBS)
dump_LIB_DIR 				+= ..
dump_INC_DIR 				+= ..
dump_CXFLAGS 				+= -DTB_PRINT_TAG=\"itrace\" 
dump_CXFLAGS-$(DEBUG) 		+= -DTB_TRACE_ENABLE -DTB_ASSERT_ENABLE
dump_SUFFIX_CMD1 			+= $(TB_CONFIG_DEMO_CMD1)

# test
test_TYPE 					= BIN
test_M_FILES 				+= test
test_LIBS 					+= tbox $(TB_CONFIG_DEMO_LIBS)
test_LDFLAGS 				+= -F$(SDKROOT)/System/Library/PrivateFrameworks -framework Foundation -L$(SRC_DIR)/
test_LIB_DIR 				+= ..
test_INC_DIR 				+= ..
test_MXFLAGS 				+= -DTB_PRINT_TAG=\"itrace\" 
test_MXFLAGS-$(DEBUG) 		+= -DTB_TRACE_ENABLE -DTB_ASSERT_ENABLE
test_SUFFIX_CMD1 			+= $(TB_CONFIG_DEMO_CMD1)

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

