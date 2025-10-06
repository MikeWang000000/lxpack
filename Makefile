ifneq ($(MAKECMDGOALS),clean)
ifndef PAYLOAD
$(error variable PAYLOAD must be defined)
endif
endif

PROG := $(basename $(notdir $(PAYLOAD)))
SRC := main.c
BUILD_DIR := $(CURDIR)/build
PROG_BIN := $(BUILD_DIR)/$(PROG)
PAYLOAD_LZMA := $(BUILD_DIR)/payload.lzma
PAYLOAD_C := $(BUILD_DIR)/payload.c
PREFIX := /usr/local
BINDIR := $(PREFIX)/bin

CFLAGS := -std=c11 -Oz -DNDEBUG -Wall -Wextra -fno-inline -fwrapv \
          -fno-unit-at-a-time -fomit-frame-pointer -fmerge-all-constants \
          -ffunction-sections -fdata-sections

ifndef STDLIB
CFLAGS := $(CFLAGS) -DNOSTDLIB -ffreestanding
endif

LDFLAGS := -s -static -lgcc \
           -Wl,--gc-sections,--build-id=none,-R,.comment \
           -Wl,-z,noseparate-code,-z,norelro,-z,nodlopen,-z,nosectionheader

ifndef STDLIB
LDFLAGS := $(LDFLAGS) -nostdlib -nostartfiles
endif

all: $(PROG_BIN)

clean:
	$(RM) -r '$(BUILD_DIR)'

install: $(PROG_BIN)
	mkdir -p '$(DESTDIR)$(BINDIR)'
	install -m 755 '$<' '$(DESTDIR)$(BINDIR)/$(PROG)'

$(PAYLOAD_LZMA): $(PAYLOAD)
	mkdir -p '$(BUILD_DIR)'
	lzma -6ec '$<' > '$@'

$(PAYLOAD_C): $(PAYLOAD_LZMA) $(PAYLOAD)
	xxd -n 'payload_lzma' -i '$<' > '$@'
	echo 'unsigned int payload_len = $(shell du -b '$(PAYLOAD)' | cut -f1);' \
		>> '$@'

$(PROG_BIN): $(SRC) $(PAYLOAD_C)
	$(CROSS_COMPILE)gcc $(CFLAGS) -o '$@' $^ $(LDFLAGS)
	echo '------'; du -h '$(PAYLOAD)' '$@'
