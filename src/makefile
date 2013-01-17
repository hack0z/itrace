# sub-makefile
include $(PRO_DIR)/prefix.mak

# ############################################################################
# names
# #
NAMES 							= itrace

# root
ifeq ($(PLAT),ios)
SDKROOT 						= /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$(SDK).sdk/
endif

ifeq ($(PLAT),mac)
SDKROOT 						= /Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$(SDK).sdk/
endif

# framework
FWKROOT 						= $(SDKROOT)/System/Library/Frameworks

# itrace
itrace_TYPE 					= DLL
itrace_M_FILES					+= itrace
itrace_LIBS						+= tbox
itrace_MXFLAGS 					+= -DTB_PRINT_TAG=\"itrace\" 
itrace_MXFLAGS-$(DEBUG) 		+= -DTB_TRACE_ENABLE -DTB_ASSERT_ENABLE
itrace_SHFLAGS					+= -F$(SDKROOT)/System/Library/PrivateFrameworks -framework Foundation -framework CoreFoundation -L$(SRC_DIR)/

# includes
INC_DIR 						+= $(FWKROOT)/Foundation.framework/Headers/ \
								   $(FWKROOT)/CoreFoundation.framework/Headers/ \
								   $(FWKROOT)/CoreGraphics.framework/Headers/

# demo
SUB_PROS 						= demo

# suffix
include $(PRO_DIR)/suffix.mak

