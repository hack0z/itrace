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
demo_LIBS 					+= tbox$(DTYPE) $(IH_CONFIG_DEMO_LIBS)
demo_LIB_DIR 				+= ..
demo_INC_DIR 				+= ..
demo_SUFFIX_CMD1 			+= $(IH_CONFIG_DEMO_CMD1)

# dump
dump_TYPE 					= BIN
dump_C_FILES 				+= dump
dump_LIBS 					+= tbox$(DTYPE) $(IH_CONFIG_DEMO_LIBS)
dump_LIB_DIR 				+= ..
dump_INC_DIR 				+= ..
dump_SUFFIX_CMD1 			+= $(IH_CONFIG_DEMO_CMD1)

# test
test_TYPE 					= BIN
test_M_FILES 				+= test
test_LIBS 					+= tbox$(DTYPE) $(IH_CONFIG_DEMO_LIBS)
test_LDFLAGS 				+= -F$(SDKROOT)/System/Library/PrivateFrameworks -framework Foundation -L$(SRC_DIR)/
test_LIB_DIR 				+= ..
test_INC_DIR 				+= ..
test_SUFFIX_CMD1 			+= $(IH_CONFIG_DEMO_CMD1)

# itrace
itrace_TYPE 				= BIN
itrace_C_FILES 				+= itrace
itrace_LIBS 				+= tbox$(DTYPE) $(IH_CONFIG_DEMO_LIBS)
itrace_LIB_DIR 				+= ..
itrace_INC_DIR 				+= ..
itrace_SUFFIX_CMD1 			+= $(IH_CONFIG_DEMO_CMD1)

# suffix
include $(PRO_DIR)/suffix.mak

