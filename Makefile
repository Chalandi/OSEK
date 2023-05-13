# ******************************************************************************************
#   Filename    : Makefile
#
#   Author      : Chalandi Amine
#
#   Owner       : Chalandi Amine
#
#   Date        : 22.11.2022
#
#   Description : Build system
#
# ******************************************************************************************

############################################################################################
# Defines
############################################################################################

HW_TARGET  = stm32f100
PRJ_NAME   = Osek_$(HW_TARGET)
OUTPUT_DIR = $(CURDIR)/Output
OBJ_DIR    = $(CURDIR)/Tmp/Obj
SRC_DIR    = $(CURDIR)

CC_ERR_FORMAT_SCRIPT = CompilerErrorFormater.py
LD_SCRIPT            = $(SRC_DIR)/stm32f100.ld

############################################################################################
# Toolchain
############################################################################################

AS      = arm-none-eabi-g++
CC      = arm-none-eabi-g++
CPP     = arm-none-eabi-g++
CPPFILT = arm-none-eabi-c++filt
LD      = arm-none-eabi-g++
NM      = arm-none-eabi-nm
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy
READELF = arm-none-eabi-readelf
PYTHON  = python3

############################################################################################
# C Compiler flags
############################################################################################

OPS_BASE     = -Wall                                          \
               -Wextra                                        \
               -Wpedantic                                     \
               -Wno-unused-function                           \
               -Wno-variadic-macros                           \
               -O2                                            \
               -mcpu=cortex-m3                                \
               -mtune=cortex-m3                               \
               -mthumb                                        \
               -mfloat-abi=soft                               \
               -mno-unaligned-access                          \
               -mno-long-calls                                \
               -fno-exceptions                                \
               -fno-inline-functions                          \
               -gdwarf-2

COPS         = -x c                                           \
               $(OPS_BASE)                                    \
               -std=c99

############################################################################################
# C++ Compiler flags
############################################################################################

CPPOPS       = -x c++                                         \
               $(OPS_BASE)                                    \
               -std=c++14                                     \
               -fno-rtti                                      \
               -fno-use-cxa-atexit                            \
               -fno-nonansi-builtins                          \
               -fno-threadsafe-statics                        \
               -finline-functions                             \
               -finline-limit=32                              \
               -Wzero-as-null-pointer-constant

############################################################################################
# Assembler flags
############################################################################################

ASOPS        = -x assembler                                   \
               $(OPS_BASE)

############################################################################################
# Linker flags
############################################################################################

LOPS         = -x none                                        \
               -nostartfiles                                  \
               -nostdlib                                      \
               -specs=nano.specs                              \
               -specs=nosys.specs                             \
               -e __my_startup                                \
               $(OPS_BASE)                                    \
               -Wl,--print-memory-usage                       \
               -Wl,-Map,$(OUTPUT_DIR)/$(PRJ_NAME).map         \
               -T $(LD_SCRIPT)


############################################################################################
# Source Files
############################################################################################

SRC_FILES :=  $(SRC_DIR)/Application/Appli                                                 \
              $(SRC_DIR)/Application/MCAL/SysTickTimer                                     \
              $(SRC_DIR)/Application/OS/OS                                                 \
              $(SRC_DIR)/Application/OS/OsAlarm                                            \
              $(SRC_DIR)/Application/OS/OsEvt                                              \
              $(SRC_DIR)/Application/OS/OsAsm                                              \
              $(SRC_DIR)/Application/OS/OsTask                                             \
              $(SRC_DIR)/Application/OS/TCB


############################################################################################
# Include Paths
############################################################################################
INC_FILES :=  $(SRC_DIR)/Application/OS/HwPlatform/STM32               \
              $(SRC_DIR)/Application/OS                                \
              $(SRC_DIR)/Application/MCAL                              \
              $(SRC_DIR)/Application

############################################################################################
# Rules
############################################################################################

VPATH := $(subst \,/,$(sort $(dir $(SRC_FILES)) $(OBJ_DIR)))

FILES_O := $(addprefix $(OBJ_DIR)/, $(notdir $(addsuffix .o, $(SRC_FILES))))


ifeq ($(MAKECMDGOALS),build)
-include $(subst .o,.d,$(FILES_O))
endif

build : clean $(OUTPUT_DIR)/$(PRJ_NAME).elf

all : clean $(OUTPUT_DIR)/$(PRJ_NAME).elf


.PHONY : clean
clean :
	@-rm -f $(OBJ_DIR)/*.o            2>/dev/null || true
	@-rm -f $(OBJ_DIR)/*.err          2>/dev/null || true
	@-rm -f $(OUTPUT_DIR)/kernel.img  2>/dev/null || true
	@-rm -f $(OUTPUT_DIR)/*.hex       2>/dev/null || true
	@-rm -f $(OUTPUT_DIR)/*.elf       2>/dev/null || true
	@-rm -f $(OUTPUT_DIR)/*.list      2>/dev/null || true
	@-rm -f $(OUTPUT_DIR)/*.map       2>/dev/null || true
	@-rm -f $(OUTPUT_DIR)/*.txt       2>/dev/null || true
	@-mkdir -p $(OBJ_DIR)
	@-mkdir -p $(OUTPUT_DIR)

$(OBJ_DIR)/%.o : %.c
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(CC) $(COPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OBJ_DIR)/%.o : %.s
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(AS) $(ASOPS) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err >$(OBJ_DIR)/$(basename $(@F)).lst
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OBJ_DIR)/%.o : %.cpp
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(CPP) $(CPPOPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OUTPUT_DIR)/$(PRJ_NAME).elf : $(FILES_O)
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O ihex $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O binary $(OUTPUT_DIR)/kernel.img
	@$(NM) --numeric-sort --print-size $(OUTPUT_DIR)/$(PRJ_NAME).elf | $(CPPFILT) > $(OUTPUT_DIR)/$(PRJ_NAME)_cppfilt.txt
