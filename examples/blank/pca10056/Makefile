PROJECT_NAME     := nsd_blank_example_pca10056
TARGETS          := nrf52840_xxaa
OUTPUT_DIRECTORY := _build

CMSIS_ROOT := ../../../env/cmsis/include
PROJ_DIR := ..
NSD_ROOT := ../../../nsd
COMPILER_ROOT := ../../../env/toolchain

# To override compiler path
TOOLCHAIN_COMMON := ../../../env/SDK_glue

include $(NSD_ROOT)/nsd.mk
include $(NSD_ROOT)/device/nsd_nRF52840.mk
INC_FOLDERS += $(NSD_INC_FOLDERS)
SRC_FILES += $(NSD_SRC_FILES)

$(OUTPUT_DIRECTORY)/nrf52840_xxaa.out: \
  LINKER_SCRIPT  := nsd_blank_example.ld

# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/main.c

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR) \
  $(CMSIS_ROOT)

# Libraries common to all targets
LIB_FILES += \

# C flags common to all targets
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS +=  -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums

# C++ flags common to all targets
CXXFLAGS += \

# Assembler flags common to all targets
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET

# Linker flags
LDFLAGS += -mthumb -mabi=aapcs -L $(LINKFILE_COMMON) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys


.PHONY: $(TARGETS) default all clean help flash

# Default target - first one defined
default: nrf52840_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo 	nrf52840_xxaa

include $(TOOLCHAIN_COMMON)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

# Flash the program
flash: $(OUTPUT_DIRECTORY)/nrf52840_xxaa.hex
	@echo Flashing: $<
	nrfjprog --program $< -f nrf52 --sectorerase
	nrfjprog --reset -f nrf52

erase:
	nrfjprog --eraseall -f nrf52