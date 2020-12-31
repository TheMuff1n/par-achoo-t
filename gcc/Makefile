# Objektdateien fuer das Hauptprogramm
_OBJS +=  \
main.o

# Objektdateien fuer Tests
_TOBJS +=  \
test/test.o

################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
src  \
src/main  \
src/test

# List the object files
OBJS = $(patsubst %, src/main/%, $(_OBJS))
TOBJS = $(patsubst %, src/%, $(_TOBJS))

OBJS_AS_ARGS += $(patsubst %, $(QUOTE)%$(QUOTE), $(OBJS))
TOBJS_AS_ARGS += $(patsubst %, $(QUOTE)%$(QUOTE), $(TOBJS))

# List the dependency files
DEPS := $(OBJS:%.o=%.d)
TDEPS := $(TOBJS:%.o=%.d)

DEPS_AS_ARGS += $(patsubst %, $(QUOTE)%$(QUOTE), $(DEPS))
TDEPS_AS_ARGS +=  $(patsubst %, $(QUOTE)%$(QUOTE), $(TDEPS))

MAKEFILE_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
OUTPUT_FILE_NAME :=AtmelStart
TOUTPUT_FILE_NAME :=AtmelTest
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf
TOUTPUT_FILE_PATH +=$(TOUTPUT_FILE_NAME).elf

vpath %.c ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)
test: $(SUB_DIRS) $(TOUTPUT_FILE_PATH)


# Linker target




$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker
	$(QUOTE)avr-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS)  -Wl,--start-group -lm -Wl,--end-group \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" -Wl,--gc-sections \
-mmcu=atmega328p -B "$(MAKEFILE_DIR)../gcc/dev/atmega328p" \
-L"../gcc/dev/atmega328p/avr5"  \
-l:libatmega328p.a 
	@echo Finished building target: $@

	"avr-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"avr-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"avr-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"avr-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"avr-size" "$(OUTPUT_FILE_NAME).elf"

$(TOUTPUT_FILE_PATH): $(TOBJS)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker
	$(QUOTE)avr-gcc$(QUOTE) -o $(TOUTPUT_FILE_NAME).elf $(TOBJS_AS_ARGS)  -Wl,--start-group -lm -Wl,--end-group \
-Wl,-Map="$(TOUTPUT_FILE_NAME).map" -Wl,--gc-sections \
-mmcu=atmega328p -B "$(MAKEFILE_DIR)../gcc/dev/atmega328p" \
-L"../gcc/dev/atmega328p/avr5"  \
-l:libatmega328p.a 
	@echo Finished building target: $@

	"avr-objcopy" -O binary "$(TOUTPUT_FILE_NAME).elf" "$(TOUTPUT_FILE_NAME).bin"
	"avr-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures \
        "$(TOUTPUT_FILE_NAME).elf" "$(TOUTPUT_FILE_NAME).hex"
	"avr-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(TOUTPUT_FILE_NAME).elf" \
        "$(TOUTPUT_FILE_NAME).eep" || exit 0
	"avr-objdump" -h -S "$(TOUTPUT_FILE_NAME).elf" > "$(TOUTPUT_FILE_NAME).lss"
	"avr-size" "$(TOUTPUT_FILE_NAME).elf"

# Compiler target(s)




%.o: %.c
	@echo Building file: $<
	@echo AVR/GNU C Compiler
	$(QUOTE)avr-gcc$(QUOTE) -x c -DF_CPU=16000000 -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=gnu99 \
-mmcu=atmega328p -B "$(MAKEFILE_DIR)../gcc/dev/atmega328p" \
    -D__mega328P__ \
-I"../" -I"../src/main"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

	@echo Finished building: $<


# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(TOBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(TOUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(TDEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec
	rm -f $(TOUTPUT_FILE_NAME).a $(TOUTPUT_FILE_NAME).hex $(TOUTPUT_FILE_NAME).bin \
        $(TOUTPUT_FILE_NAME).lss $(TOUTPUT_FILE_NAME).eep $(TOUTPUT_FILE_NAME).map \
        $(TOUTPUT_FILE_NAME).srec
