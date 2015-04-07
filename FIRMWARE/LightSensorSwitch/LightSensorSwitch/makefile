MCU          = atmega48
ARCH         = AVR8
F_CPU        = 1000000
OPTIMIZATION = s
C_STANDARD   = gnu99
CPP_STANDARD = gnu++98
DEBUG_LEVEL  = 2
LINKER_RELAXATIONS = Y
TARGET       = LightSensorSwitch
SRC          = $(TARGET).c leds.c light_sensor.c state_day.c state_night.c states.c watchdog.c voltage_sensor.c events.c ext_load.c
LD_FLAGS     =

# Output Messages
MSG_INFO_MESSAGE := ' [INFO]    :'
MSG_COMPILE_CMD  := ' [GCC]     :'
MSG_ASSEMBLE_CMD := ' [GAS]     :'
MSG_NM_CMD       := ' [NM]      :'
MSG_REMOVE_CMD   := ' [RM]      :'
MSG_LINK_CMD     := ' [LNK]     :'
MSG_ARCHIVE_CMD  := ' [AR]      :'
MSG_SIZE_CMD     := ' [SIZE]    :'
MSG_OBJCPY_CMD   := ' [OBJCPY]  :'
MSG_OBJDMP_CMD   := ' [OBJDMP]  :'
CROSS        	 := avr

# Convert input source file list to differentiate them by type
C_SOURCE   := $(filter %.c, $(SRC))
CPP_SOURCE := $(filter %.cpp, $(SRC))
ASM_SOURCE := $(filter %.S, $(SRC))

# Convert input source filenames into a list of required output object files
OBJECT_FILES += $(addsuffix .o, $(basename $(SRC)))

# Create a list of dependency files from the list of object files
DEPENDENCY_FILES := $(OBJECT_FILES:%.o=%.d)

# Create a list of common flags to pass to the compiler/linker/assembler
BASE_CC_FLAGS    := -pipe -g$(DEBUG_FORMAT) -g$(DEBUG_LEVEL)
BASE_CC_FLAGS += -mmcu=$(MCU) -fshort-enums -fno-inline-small-functions -fpack-struct
BASE_CC_FLAGS += -Wall -fno-strict-aliasing -funsigned-char -funsigned-bitfields -ffunction-sections
BASE_CC_FLAGS += -I. -I$(patsubst %/,%,$(LUFA_PATH))/..
BASE_CC_FLAGS += -DARCH=ARCH_$(ARCH) 
ifneq ($(F_CPU),)
   BASE_CC_FLAGS += -DF_CPU=$(F_CPU)UL
endif

# Additional language specific compiler flags
BASE_C_FLAGS   := -x c -O$(OPTIMIZATION) -std=$(C_STANDARD) -Wstrict-prototypes
BASE_CPP_FLAGS := -x c++ -O$(OPTIMIZATION) -std=$(CPP_STANDARD)
BASE_ASM_FLAGS := -x assembler-with-cpp

# Create a list of flags to pass to the linker
BASE_LD_FLAGS := -lm -Wl,-Map=$(TARGET).map,--cref -Wl,--gc-sections
ifeq ($(LINKER_RELAXATIONS), Y)
   BASE_LD_FLAGS += -Wl,--relax
endif
   BASE_LD_FLAGS += -mmcu=$(MCU)

# Determine flags to pass to the size utility based on its reported features (only invoke if size target required)
# and on an architecture where this non-standard patch is available
ifneq ($(ARCH), UC3)
size: SIZE_MCU_FLAG    := $(shell $(CROSS)-size --help | grep -- --mcu > /dev/null && echo --mcu=$(MCU) )
size: SIZE_FORMAT_FLAG := $(shell $(CROSS)-size --help | grep -- --format=.*avr > /dev/null && echo --format=avr )
endif


# Performs a complete build of the user application and prints size information afterwards
all: build_begin elf hex bin lss sym size build_end

clean: mostlyclean
	@echo $(MSG_REMOVE_CMD) Removing output files of \"$(TARGET)\"
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).eep $(TARGET).map $(TARGET).lss $(TARGET).sym $(TARGET).a

# Cleans intermediary build files, leaving only the compiled application files
mostlyclean:
	@echo $(MSG_REMOVE_CMD) Removing object files of \"$(TARGET)\"
	rm -f $(OBJECT_FILES)
	@echo $(MSG_REMOVE_CMD) Removing dependency files of \"$(TARGET)\"
	rm -f $(DEPENDENCY_FILES)

# Pre-build informational target, to give compiler and project name information when building
build_begin:
	@echo $(MSG_INFO_MESSAGE) Begin compilation of project \"$(TARGET)\"...
	@echo ""
	@$(CROSS)-gcc --version

# Post-build informational target, to project name information when building has completed
build_end:
	@echo $(MSG_INFO_MESSAGE) Finished building project \"$(TARGET)\".

# Prints size information of a compiled application (FLASH, RAM and EEPROM usages)
size: $(TARGET).elf
	@echo $(MSG_SIZE_CMD) Determining size of \"$<\"
	@echo ""
	$(CROSS)-size $(SIZE_MCU_FLAG) $(SIZE_FORMAT_FLAG) $<

# Helper targets, to build a specific type of output file without having to know the project target name
lib: lib$(TARGET).a
elf: $(TARGET).elf
hex: $(TARGET).hex $(TARGET).eep
bin: $(TARGET).bin $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym

# Default target to *create* the user application's specified source files; if this rule is executed by
# make, the input source file doesn't exist and an error needs to be presented to the user
$(SRC):
	$(error Source file does not exist: $@)

# Compiles an input C source file and generates an assembly listing for it
%.s: %.c $(MAKEFILE_LIST)
	@echo $(MSG_COMPILE_CMD) Generating assembly from C file \"$(notdir $<)\"
	$(CROSS)-gcc -S $(BASE_CC_FLAGS) $(BASE_C_FLAGS) $(CC_FLAGS) $(C_FLAGS) $< -o $@

# Compiles an input C++ source file and generates an assembly listing for it
%.s: %.cpp $(MAKEFILE_LIST)
	@echo $(MSG_COMPILE_CMD) Generating assembly from C++ file \"$(notdir $<)\"
	$(CROSS)-gcc -S $(BASE_CC_FLAGS) $(BASE_CPP_FLAGS) $(CC_FLAGS) $(CPP_FLAGS) $< -o $@

# Compiles an input C source file and generates a linkable object file for it
%.o: %.c $(MAKEFILE_LIST)
	@echo $(MSG_COMPILE_CMD) Compiling C file \"$(notdir $<)\"
	$(CROSS)-gcc -c $(BASE_CC_FLAGS) $(BASE_C_FLAGS) $(CC_FLAGS) $(C_FLAGS) -MMD -MP -MF $(@:%.o=%.d) $< -o $@

# Compiles an input C++ source file and generates a linkable object file for it
%.o: %.cpp $(MAKEFILE_LIST)
	@echo $(MSG_COMPILE_CMD) Compiling C++ file \"$(notdir $<)\"
	$(CROSS)-gcc -c $(BASE_CC_FLAGS) $(BASE_CPP_FLAGS) $(CC_FLAGS) $(CPP_FLAGS) -MMD -MP -MF $(@:%.o=%.d) $< -o $@

# Assembles an input ASM source file and generates a linkable object file for it
%.o: %.S $(MAKEFILE_LIST)
	@echo $(MSG_ASSEMBLE_CMD) Assembling \"$(notdir $<)\"
	$(CROSS)-gcc -c $(BASE_CC_FLAGS) $(BASE_ASM_FLAGS) $(CC_FLAGS) $(ASM_FLAGS) -MMD -MP -MF $(@:%.o=%.d) $< -o $@

# Generates a library archive file from the user application, which can be linked into other applications
.PRECIOUS  : $(OBJECT_FILES)
.SECONDARY : %.a
%.a: $(OBJECT_FILES)
	@echo $(MSG_ARCHIVE_CMD) Archiving object files into \"$@\"
	$(CROSS)-ar rcs $@ $(OBJECT_FILES)

# Generates an ELF debug file from the user application, which can be further processed for FLASH and EEPROM data
# files, or used for programming and debugging directly
.PRECIOUS  : $(OBJECT_FILES)
.SECONDARY : %.elf
%.elf: $(OBJECT_FILES)
	@echo $(MSG_LINK_CMD) Linking object files into \"$@\"
	$(CROSS)-gcc $^ -o $@ $(BASE_LD_FLAGS) $(LD_FLAGS)

# Extracts out the loadable FLASH memory data from the project ELF file, and creates an Intel HEX format file of it
%.hex: %.elf
	@echo $(MSG_OBJCPY_CMD) Extracting HEX file data from \"$<\"
	$(CROSS)-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

# Extracts out the loadable FLASH memory data from the project ELF file, and creates an Binary format file of it
%.bin: %.elf
	@echo $(MSG_OBJCPY_CMD) Extracting BIN file data from \"$<\"
	$(CROSS)-objcopy -O binary -R .eeprom -R .fuse -R .lock -R .signature $< $@

# Extracts out the loadable EEPROM memory data from the project ELF file, and creates an Intel HEX format file of it
%.eep: %.elf
	@echo $(MSG_OBJCPY_CMD) Extracting EEP file data from \"$<\"
	$(CROSS)-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@ || exit 0

# Creates an assembly listing file from an input project ELF file, containing interleaved assembly and source data
%.lss: %.elf
	@echo $(MSG_OBJDMP_CMD) Extracting LSS file data from \"$<\"
	$(CROSS)-objdump -h -d -S -z $< > $@

# Creates a symbol file listing the loadable and discarded symbols from an input project ELF file
%.sym: %.elf
	@echo $(MSG_NM_CMD) Extracting SYM file data from \"$<\"
	$(CROSS)-nm -n $< > $@

# Phony build targets for this module
.PHONY: build_begin build_end size symbol-sizes lib elf hex lss clean mostlyclean


