#
# Please excuse this horrible mess for now!
#
# This is the result of merging features from 4 different AVR makefiles; havent
# had a chance to clean it up yet. Sorry.
#

# MCU name
MCU = atmega328p
F_CPU = 20000000
PORT = /dev/arduino2
#BAUD_RATE = 38400
BAUD_RATE = 115200

# hfuse 11010010 = 0xd2


NORMAL_LFUSE = 0xff
NORMAL_HFUSE = 0xdf
NORMAL_EFUSE = 0x04

DEBUGGING_LFUSE = 0xff
DEBUGGING_HFUSE = 0x9f
DEBUGGING_EFUSE = 0x04


# Output format. (can be srec, ihex, binary)
FORMAT = ihex 

# Target file name (without extension).
TARGET = ledcube

# Optimization level, can be [0, 1, 2, 3, s]. 0 turns off optimization.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
#OPT = 0
#OPT = s
OPT = 3


# List C source files here. (C dependencies are automatically generated.)

SRC = \
	  $(TARGET).c driver.c color.c random_bugs.c cubes.c

CXXSRC = 

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = asmdriver.S


# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS = 


# Optional compiler flags.
#  -g:        generate debugging information (for GDB, or for COFF conversion)
#  -O*:       optimization level
#  -f...:     tuning, see gcc manual and avr-libc documentation
#  -Wall...:  warning level
#  -Wa,...:   tell GCC to pass this to the assembler.
#    -ahlms:  create assembler listing
GDB		= avr-gdb
CFLAGS = -ggdb3 -O$(OPT) \
-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums \
-Wall -Wstrict-prototypes \
-DF_CPU=$(F_CPU) -DSERIAL_BAUD_RATE=$(BAUD_RATE) \
-Wa,-adhlns=$(subst $(suffix $<),.lst,$<)  \
$(patsubst %,-I%,$(EXTRAINCDIRS))


# Set a "language standard" compiler flag.
#   Unremark just one line below to set the language standard to use.
#   gnu99 = C99 + GNU extensions. See GCC manual for more information.
#CFLAGS += -std=c89
#CFLAGS += -std=gnu89
#CFLAGS += -std=c99
CFLAGS += -std=gnu99



# Optional assembler flags.
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 



# Optional linker flags.
#  -Wl,...:   tell GCC to pass this to linker.
#  -Map:      create map file
#  --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
#LDFLAGS += -lstdc++  # somehow fixes issues related to __cxa_guard_acquire/release



# Additional libraries

# Minimalistic printf version
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires -lm below)
LDFLAGS += -Wl,-u,vfprintf -lprintf_flt

# -lm = math library
LDFLAGS += -lm

AVARICE = avarice
AVARICE_MCU = $(MCU)
AVARICE_PORT = 4242
AVARICE_FLAGS = --program --file $(TARGET).elf --part $(AVARICE_MCU) --debugwire --dragon -d localhost:$(AVARICE_PORT)

AVRDUDE_PATH = /usr/local/bin
AVRDUDE_DRAGON = dragon_isp
AVRDUDE_MKII = avrispmkII
AVRDUDE_PORT = usb
AVRDUDE_CONF = /usr/local/etc/avrdude.conf
AVRDUDE_VERBOSITY = -vv -B0.13
AVRDUDE_MCU = m328p

# AVRDUDE Settings
AVRDUDE_WRITE_FLASH = -Uflash:w:$(TARGET).hex:i
AVRDUDE_WRITE_NORMAL_HFUSE = -Uhfuse:w:$(NORMAL_HFUSE):m
AVRDUDE_WRITE_NORMAL_LFUSE = -Ulfuse:w:$(NORMAL_LFUSE):m
AVRDUDE_WRITE_NORMAL_EFUSE = -Uefuse:w:$(NORMAL_EFUSE):m
AVRDUDE_WRITE_DEBUGGING_HFUSE = -Uhfuse:w:$(DEBUGGING_HFUSE):m
AVRDUDE_WRITE_DEBUGGING_LFUSE = -Ulfuse:w:$(DEBUGGING_LFUSE):m
AVRDUDE_WRITE_DEBUGGING_EFUSE = -Uefuse:w:$(DEBUGGING_EFUSE):m

AVRDUDE_NORMAL_FUSES_FLAGS = $(AVRDUDE_WRITE_NORMAL_HFUSE) $(AVRDUDE_WRITE_NORMAL_LFUSE) $(AVRDUDE_WRITE_NORMAL_EFUSE)
AVRDUDE_DEBUGGING_FUSES_FLAGS = $(AVRDUDE_WRITE_DEBUGGING_HFUSE) $(AVRDUDE_WRITE_DEBUGGING_LFUSE) $(AVRDUDE_WRITE_DEBUGGING_EFUSE)

AVRDUDE_FLAGS       = $(AVRDUDE_EXTRA_FLAGS) $(AVRDUDE_VERBOSITY) -P$(AVRDUDE_PORT) -p$(AVRDUDE_MCU) -C$(AVRDUDE_CONF)

AVRDUDE = $(AVRDUDE_PATH)/avrdude


# ---------------------------------------------------------------------------

# Define directories, if needed.
DIRAVR = c:/winavr
DIRAVRBIN = $(DIRAVR)/bin
DIRAVRUTILS = $(DIRAVR)/utils/bin
DIRINC = .
DIRLIB = $(DIRAVR)/avr/lib


# Define programs and commands.
#
AVRPATH = /usr/local/bin
SHELL = sh

CC = $(AVRPATH)/avr-gcc
CXX = $(AVRPATH)/avr-g++

OBJCOPY = $(AVRPATH)/avr-objcopy
OBJDUMP = $(AVRPATH)/avr-objdump
SIZE = $(AVRPATH)/avr-size



REMOVE = rm -f
COPY = cp

HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:

INCLUDE = -I. $(EXTRAINCDIRS)

# Define all object files.
OBJ = $(SRC:%.c=%.o) $(ASRC:%.S=%.o) $(CXXSRC:%.cpp=%.o)

# Define all listing files.
LST = $(ASRC:%.S=%.lst) $(SRC:%.c=%.lst) $(CXXSRC:%.cpp=%.lst)

ALL = $(OBJ) $(LST)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) $(INCLUDE) $(CFLAGS)  -lm
ALL_ASFLAGS = -mmcu=$(MCU) $(INCLUDE) -x assembler-with-cpp $(ASFLAGS)  -lm



# Default target.
all: begin gccversion sizebefore $(TARGET).elf $(TARGET).hex $(TARGET).eep \
	$(TARGET).lss $(TARGET).sym sizeafter finished end

# Program the device.
program: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c$(AVRDUDE_MKII) $(AVRDUDE_WRITE_FLASH) || \
	$(AVRDUDE) $(AVRDUDE_FLAGS) -V -c$(AVRDUDE_DRAGON) $(AVRDUDE_WRITE_FLASH) 
program2: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -c$(AVRDUDE_MKII) $(AVRDUDE_WRITE_FLASH)

fuses:
	$(AVRDUDE) -c$(AVRDUDE_DRAGON) $(AVRDUDE_FLAGS) $(AVRDUDE_NORMAL_FUSES_FLAGS) ; sleep 5; \
	$(AVRDUDE) -c$(AVRDUDE_DRAGON) $(AVRDUDE_FLAGS) $(AVRDUDE_NORMAL_FUSES_FLAGS)
debug-fuses:
	$(AVRDUDE) -c$(AVRDUDE_DRAGON) $(AVRDUDE_FLAGS) $(AVRDUDE_DEBUGGING_FUSES_FLAGS) \
		@echo "Power cycle the target to enable debugwire"

debug: $(TARGET).elf 
	$(AVARICE) $(AVARICE_FLAGS)
gdb:
	$(GDB) $(TARGET).elf


# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE); \
		$(HEXSIZE)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi



# Display compiler version information.
gccversion : 
	@$(CC) --version




# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	avr-nm -n $< > $@


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS) 

# Compile: create object files from C++ source files.
%.o : %.cpp
	$(CXX) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create object files from C source files.
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) -DRANDOM_SEED=$(shell date +%s)   $< -o $@ 


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@ 


# Assemble: create object files from assembler source files.
%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@ 

# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) -v $(TARGET).map $(ALL) $(TARGET).hex $(TARGET).eep $(SRC:.c=.s) $(SRC:.c=.d) $(TARGET).elf $(TARGET).lss $(TARGET).sym

# Automatically generate C source code dependencies. 
# (Code originally taken from the GNU make user manual and modified 
# (See README.txt Credits).)
#
# Note that this will work with sh (bash) and sed that is shipped with WinAVR
# (see the SHELL variable defined above).
# This may not work with other shells or other seds.

%.d: %.c
	set -e; $(CC) -MM $(ALL_CFLAGS) $< \
	| sed 's,\(.*\)\.o[ :]*,\1.o \1.d : ,g' > $@; \
	[ -s $@ ] || rm -f $@


# Remove the '-' if you want to see the dependency files generated.
#include $(SRC:.c=.d)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion coff extcoff \
	clean clean_list program
